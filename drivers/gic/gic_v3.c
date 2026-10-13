/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#include "gic.h"
#include "gic_v3.h"
#include "opena64/arm.h"
#include "opena64/mmio.h"
#include "opena64/assert.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef GICR_NUM_CORE
#define GICR_NUM_CORE (4)
#endif

#ifndef GICD_BASE_ADDRESS
#define GICD_BASE_ADDRESS 0x08000000UL
#endif

#ifndef GICR_BASE_ADDRESS
#define GICR_BASE_ADDRESS 0x080A0000UL
#endif

/** non-secure mode */
#define IGROUPR_VAL 0xFFFFFFFFU

static uintptr_t gicv3_dist = GICD_BASE_ADDRESS;
static uintptr_t gicv3_rdist[GICR_NUM_CORE] = {
	0x080A0000UL,
};

static inline uint32_t get_cpu_id(void)
{
	return 0;
}

static inline uintptr_t gicr_rdist_base_addr(void)
{
	return gicv3_rdist[get_cpu_id()];
}

static inline uint32_t gicv3_mpidr_affinity(uint64_t mpidr)
{
	uint32_t aff0 = (uint32_t)(mpidr & 0xFFU);
	uint32_t aff1 = (uint32_t)((mpidr >> 8) & 0xFFU);
	uint32_t aff2 = (uint32_t)((mpidr >> 16) & 0xFFU);
	uint32_t aff3 = (uint32_t)((mpidr >> 32) & 0xFFU);

	return (aff3 << 24) | (aff2 << 16) | (aff1 << 8) | aff0;
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
	mmio_write_u32(GICD_CTLR(gicv3_dist), 0);
	wait_rwp(0);

	num_int_line = mmio_read_u32(GICD_TYPER(gicv3_dist));
	for (int i = 1; i < (num_int_line + 1); i++) {
		mmio_write_u32(GICD_ICENABLERn(gicv3_dist, i), 0);
		mmio_write_u32(GICD_ICPENDRn(gicv3_dist, i), 0);
		mmio_write_u32(GICD_IGROUPRn(gicv3_dist, i), IGROUPR_VAL);
		mmio_write_u32(GICD_IGROUPMODRn(gicv3_dist, i), 0);
	}

	for (int i = 2; i < (2 * (num_int_line + 1)); i++) {
		// set to trigger sensitive by default
		mmio_write_u32(GICD_ICFGRn(gicv3_dist, i), 0x55555555);
	}

	// enable group0 (non-secure state)
	// NOTE: that for now this GIC driver supports Non-Secure mode
	// so there is only two groups, group1 and group1NS
	// the view of this register is different in non-secure mode
	// (SCR_EL3.NS == 1).
	//
	// The ARE_NS shares the bit position of ARE_S, and we only want
	// to enable G1NS for the distributor
	mmio_bit_set_u32(GICD_CTLR(gicv3_dist), GICD_CTRL_ARE_S | GICD_CTLR_ENABLE_G1A);
	wait_rwp(0);
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
	// level-triggered
	mmio_write_u32(GICR_ICFGR0(base_addr), 0);
	mmio_write_u32(GICR_ICFGR1(base_addr), 0);
	// group set to NS
	// TODO: secured state routing and grouping
	mmio_write_u32(GICR_IGROUPR0(base_addr), IGROUPR_VAL);

	// clear the GICR_WAKER.ProcessorSleep then
	// poll GICR_WAKER.ChildenAslee until it reads `0`
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
	if (icc_sre & ICC_SRE_SRE_ENABLE) {
		icc_sre = ICC_SRE_SRE_ENABLE | ICC_SRE_DFB_ENABLE | ICC_SRE_DIB_ENABLE |
			  ICC_SRE_LOW_EL_ENABLE;
		sys_reg_write(ICC_SRE_EL1, icc_sre);

		// test if SRE was actually enabled
		__ASSERT(sys_reg_bit_get(ICC_SRE_EL1, ICC_SRE_SRE_ENABLE));
	}

	// enable group0 interrupts
	sys_reg_bit_set(ICC_IGRPEN1_EL1, 1);
}

void gic_v3_init(void)
{
	gic_dist_init();
	gic_cpuif_init();
	gic_rdist_enable();
}

void gic_v3_int_enable(unsigned int int_id)
{
}

void gic_v3_int_disable(unsigned int int_id)
{
}

void gic_v3_int_is_enable(unsigned int int_id)
{
}

void gic_v3_int_is_pending(unsigned int int_id)
{
}

void gic_v3_int_set_pending(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ISPENDRn(GICD_BASE_ADDRESS, (int_id >> 5)),
				 (1u << (int_id % 32)));
	} else {
		uintptr_t base = gicr_rdist_base_addr();
		mmio_bit_set_u32(GICR_ISPENDR0(gicr_rdist_base_addr()), (1u << int_id));
	}
}

void gic_v3_int_clear_pending(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ICPENDRn(GICD_BASE_ADDRESS, (int_id >> 5)),
				 (1u << (int_id % 32)));
	} else {
		uintptr_t base = gicr_rdist_base_addr();
		mmio_bit_set_u32(GICR_ICPENDR0(base), (1u << int_id));
	}
}

void gic_v3_int_set_priority(unsigned int int_id, uint8_t prio)
{
	uintptr_t base = gicr_rdist_base_addr();

	if (GIC_IS_SPI(int_id)) {
		mmio_write_u32(GICD_IPRIORITYRn(base, int_id), prio);
	}
}

void gic_v3_int_set_priority_filter(uint8_t prio)
{
	// GIC architecture spec 8.5.20
	// interrupts higher than the set priority in this register
	// will be signaled to the processing entity (PE)
	sys_reg_write(ICC_PMR_EL1, prio);
}

void gic_v3_raise_sgi(unsigned int sgi_id, uint64_t target_aff, uint16_t target_list)
{
}
