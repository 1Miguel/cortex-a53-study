/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#include "gic.h"
#include "gic_v3_priv.h"
#include "arm.h"
#include "mmio.h"
#include "assert.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Interrupt handler.
 */
typedef void (*gic_irq_handler_t)(void);

/* default priority mask */
#ifndef GIC_IDLE_PRIO
#define GIC_IDLE_PRIO (0xFFU)
#endif // GIC_IDLE_PRIO

#ifndef GICR_NUM_CORE
#define GICR_NUM_CORE (4)
#endif

#ifndef GICD_BASE_ADDRESS
#define GICD_BASE_ADDRESS 0x08000000UL
#endif

#ifndef GICR_BASE_ADDRESS
#define GICR_BASE_ADDRESS 0x080A0000UL
#endif

/**
 * the group and group modifier dictates the group routing
 * the combination can be found in gic v3 and v3 specs:
 *
 * +----------+----------+----------+
 * |  groupm  |  groupr  |  group   |
 * +----------+----------+----------+
 * |    0     |    0     |  G0S     |
 * |    0     |    1     |  G1NS    |
 * |    1     |    0     |  G1S     |
 * +----------+----------+----------+
 *
 * G0S are handled in secure firmware at EL3 (FIQ)
 * G1NS or group 1 non-secure is handled by a non secured OS
 * which runs in EL1
 * G1S is handled at secure EL1
 *
 * for now, this gic driver only supports non-secure mode
 */
#define IGROUPR_VAL   0xFFFFFFFFU
#define IGROUPMOD_VAL 0x00000000U

extern gic_irq_handler_t __gic_vec_start__[];
extern gic_irq_handler_t __gic_vec_end__[];

static uintptr_t gicv3_dist = GICD_BASE_ADDRESS;
static uintptr_t gicv3_rdist[GICR_NUM_CORE] = {
	0x080A0000UL,
};

static inline uintptr_t gicr_rdist_base_addr(void)
{
	return gicv3_rdist[curr_cpu_id()];
}

static inline uint32_t gicv3_mpidr_affinity(uint64_t mpidr)
{
	uint32_t aff0 = (uint32_t)(mpidr & 0xFFU);
	uint32_t aff1 = (uint32_t)((mpidr >> 8) & 0xFFU);
	uint32_t aff2 = (uint32_t)((mpidr >> 16) & 0xFFU);
	uint32_t aff3 = (uint32_t)((mpidr >> 32) & 0xFFU);

	return (aff3 << 24) | (aff2 << 16) | (aff1 << 8) | aff0;
}

static inline void set_irouter_curr_affinity(unsigned int int_id)
{
	uint64_t mpidr = sys_reg_bit_get(MPIDR_EL1, 0xff00ffffff);

	mmio_write_u64(GICD_IROUTERn(gicv3_dist, int_id), mpidr);
}

static inline void wait_rwp(uint32_t int_id)
{
	if (GIC_IS_SPI(int_id)) {
		while (mmio_bit_get_u32(GICD_CTLR(gicv3_dist), GICD_CTLR_RWP))
			;
	} else {
		while (mmio_bit_get_u32(gicr_rdist_base_addr(), GICR_CTLR_RWP))
			;
	}
}

/**
 * @brief This routine configures the GIC distributor and configures
 * the gic global configurations.
 */
static void gic_dist_init(void)
{
	uint32_t num_int_line; // number of interrupt line
	uint32_t gicd_ctlr;

	gicd_ctlr = mmio_bit_get_u32(GICD_CTLR(gicv3_dist), GICD_CTLR_ENABLE_G0 |
								    GICD_CTLR_ENABLE_G1NS |
								    GICD_CTLR_ENABLE_G1S);
	// if groups are already enabled, then a different core has already
	// configured the gic distributor, prevent other cores to re-configure
	if (gicd_ctlr) {
		return;
	}

	// disable gic distributor
	mmio_bit_clr_u32(GICD_CTLR(gicv3_dist),
			 GICD_CTLR_ENABLE_G0 | GICD_CTLR_ENABLE_G1NS | GICD_CTLR_ENABLE_G1S);
	wait_rwp(GIC_SPI_INT_ID_START);

	num_int_line = mmio_bit_get_u32(GICD_TYPER(gicv3_dist), GICD_TYPER_ITLINESNUMBER_BIT_MSK);
	for (int i = 1; i < (num_int_line + 1); i++) {
		mmio_write_u32(GICD_ICENABLERn(gicv3_dist, i), 0xFFFFFFFF);
		mmio_write_u32(GICD_ICPENDRn(gicv3_dist, i), 0xFFFFFFFF);
		mmio_write_u32(GICD_ICACTIVERn(gicv3_dist, i), 0xFFFFFFFF);
		mmio_write_u32(GICD_IGROUPRn(gicv3_dist, i), IGROUPR_VAL);
		mmio_write_u32(GICD_IGROUPMODRn(gicv3_dist, i), IGROUPMOD_VAL);
	}

	wait_rwp(GIC_SPI_INT_ID_START);

	for (int i = 2; i < (2 * (num_int_line + 1)); i++) {
		mmio_write_u32(GICD_ICFGRn(gicv3_dist, i), 0);
		mmio_write_u32(GICD_NSACRn(gicv3_dist, i), 0xAAAAAAAA);
	}

	// enable group0 (non-secure state)
	// NOTE: that for now this GIC driver supports Non-Secure mode
	// so there is only two groups, group1 and group1NS
	// the view of this register is different in non-secure mode
	// (SCR_EL3.NS == 1).
	//
	// The ARE_NS shares the bit position of ARE_S, and we only want
	// to enable G1NS for the distributor
	mmio_bit_set_u32(GICD_CTLR(gicv3_dist), GICD_CTRL_ARE_S | GICD_CTRL_ARE_NS |
							GICD_CTLR_NS_ENABLE_G1 |
							GICD_CTLR_NS_ENABLE_G1A);
	wait_rwp(GIC_SPI_INT_ID_START);
}

/**
 * @brief enable the current cpu redistributor.
 */
static void gic_rdist_enable(void)
{
	uintptr_t base_addr = gicr_rdist_base_addr();

	if (!mmio_bit_get_u32(GICR_WAKER(base_addr), GICR_WAKER_CA_BIT_MSK)) {
		return; // GIC see PE is already online
	}

	// disable irq
	mmio_write_u32(GICR_ICENABLER0(base_addr), 0);
	// clear any pending
	mmio_write_u32(GICR_ICPENDR0(base_addr), 0);
	// default level-triggered
	mmio_write_u32(GICR_ICFGR0(base_addr), 0);
	mmio_write_u32(GICR_ICFGR1(base_addr), 0);
	mmio_write_u32(GICR_IGROUPR0(base_addr), IGROUPR_VAL);
	mmio_write_u32(GICR_IGROUPMODR0(base_addr), IGROUPMOD_VAL);

	// clear the GICR_WAKER.ProcessorSleep then
	// poll GICR_WAKER.ChildenAsleep until it reads `0`
	mmio_bit_clr_u32(GICR_WAKER(base_addr), GICR_WAKER_PS_BIT_MSK);
	while (mmio_bit_get_u32(GICR_WAKER(base_addr), GICR_WAKER_CA_BIT_MSK))
		;
}

void gic_cpuif_init(void)
{
	// for now we run at EL3, but access to EL1
	// and other lower EL levels must be set at
	// boot code (ACTLR_EL3 register)
	uint32_t icc_sre = sys_reg_read(ICC_SRE_EL1);
	if (!(icc_sre & ICC_SRE_SRE_ENABLE)) {
		icc_sre = ICC_SRE_SRE_ENABLE | ICC_SRE_DFB_ENABLE | ICC_SRE_DIB_ENABLE;
		sys_reg_write(ICC_SRE_EL1, icc_sre);
		isb();

		// test if SRE was actually enabled
		__ASSERT(sys_reg_bit_get(ICC_SRE_EL1, ICC_SRE_SRE_ENABLE));
	}

	sys_reg_write(ICC_PMR_EL1, GIC_IDLE_PRIO);
	isb();

	// enable group0 interrupts
	sys_reg_bit_set(ICC_IGRPEN1_EL1, 1);
	sys_reg_bit_set(ICC_IGRPEN0_EL1, 1);
	isb();
}

void gic_v3_init(void)
{
	gic_cpuif_init();
	gic_rdist_enable();
	gic_dist_init();
}

void gic_v3_int_enable(unsigned int int_id)
{
	set_irouter_curr_affinity(int_id);

	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ISENABLERn(gicv3_dist, int_id >> 5u), (1U << (int_id % 32)));
	} else {
		mmio_bit_set_u32(GICR_ISENABLER0(gicr_rdist_base_addr()), (1U << int_id));
	}
	wait_rwp(int_id);
}

void gic_v3_int_disable(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ICENABLERn(gicv3_dist, int_id >> 5u), (1U << (int_id % 32u)));
	} else {
		mmio_bit_set_u32(GICR_ICENABLER0(gicr_rdist_base_addr()), (1U << int_id));
	}
}

bool gic_v3_int_is_enable(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		return mmio_bit_get_u32(GICD_ISPENDRn(gicv3_dist, int_id >> 5u),
					(1U << (int_id % 32u)));
	} else {
		return mmio_bit_get_u32(GICR_ISPENDR0(gicr_rdist_base_addr()), (1U << int_id));
	}
}

bool gic_v3_int_is_pending(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		return mmio_bit_get_u32(GICD_ISPENDRn(gicv3_dist, int_id >> 5u),
					(1U << (int_id % 32u)));
	} else {
		return mmio_bit_get_u32(GICR_ISPENDR0(gicr_rdist_base_addr()), (1U << int_id));
	}
}

void gic_v3_int_set_pending(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ISPENDRn(gicv3_dist, (int_id >> 5)), (1u << (int_id % 32)));
	} else {
		uintptr_t base = gicr_rdist_base_addr();
		mmio_bit_set_u32(GICR_ISPENDR0(gicr_rdist_base_addr()), (1u << int_id));
	}
}

void gic_v3_int_clear_pending(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ICPENDRn(gicv3_dist, (int_id >> 5)), (1u << (int_id % 32)));
	} else {
		uintptr_t base = gicr_rdist_base_addr();
		mmio_bit_set_u32(GICR_ICPENDR0(base), (1u << int_id));
	}
}

void gic_v3_int_set_priority(unsigned int int_id, uint8_t prio)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_write_u8(GICD_IPRIORITYRn(gicv3_dist, int_id), prio);
	} else {
		mmio_write_u8(GICR_IPRIORITYRn(gicr_rdist_base_addr(), int_id), prio);
	}
}

void gic_v3_int_set_priority_filter(uint8_t prio)
{
	// GIC architecture spec 8.5.20
	// interrupts higher than the set priority in this register
	// will be signaled to the processing entity (PE)
	sys_reg_write(ICC_PMR_EL1, prio);
	isb();
}

void gic_v3_raise_sgi(unsigned int sgi_id, uint64_t target_aff, uint16_t target_list)
{
}

void gic_v3_irq_handler(void)
{
	uint64_t iar;
	uint32_t num_gic_handler = (__gic_vec_end__ - __gic_vec_start__);

	do {
		iar = sys_reg_read(ICC_IAR1_EL1);

		if (iar >= 1020U) {
			break;
		}

		if (iar >= num_gic_handler) {
			break;
		}

		if (__gic_vec_start__[iar]) {
			__gic_vec_start__[iar]();
		}

		isb();
		dsb();
		// group0 eoir, write the corresponding icc_iar0 interrupt id
		//
		// note: if the EOImode bit for the current Exception level and
		// Security state is 0, a write to this register drops the priority
		// for the interrupt, and also deactivates the interrupt
		sys_reg_write(ICC_EOIR1_EL1, iar);
		isb();

	} while (1);
}
