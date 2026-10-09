/*******************************************************************************
 * @file gic_v3.c
 * @brief GICv3 distributor and redistributor operations.
 *******************************************************************************/

#include "arm.h"
#include "mmio.h"
#include "gic.h"
#include "gic_v3.h"

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

static uintptr_t gicv3_dist;
static uintptr_t gicv3_rdist[GICR_NUM_CORE];

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

/**
 * @brief enable the current cpu redistributor.
 */
static void gic_rdist_enable(void)
{
	uintptr_t base_addr = gicr_rdist_base_addr();

	if (!mmio_bit_get_u32(GICR_WAKER(base_addr), GICR_WAKER_CA_BIT_MSK)) {
		return; // GIC see PE is already online
	}

	// clear the GICR_WAKER.ProcessorSleep then
	// poll GICR_WAKER.ChildenAslee until it reads `0`
	mmio_bit_clr_u32(GICR_WAKER(base_addr), GICR_WAKER_PS_BIT_MSK);
	while (mmio_bit_get_u32(GICR_WAKER(base_addr), GICR_WAKER_CA_BIT_MSK))
	;
}

void gic_cpuif_init(void)
{

}

void gic_dist_init(void)
{

  // set eoi mode for el1 and el3
  // let el3 observe and ack interrupts (RM=1)
  // disable use of binary point register
  uint32_t icc_ctrl = sys_reg_read(ICC_CTLR_EL3);
  icc_ctrl |= ICC_CTLR_RM
           | ICC_CTLR_EOI_MODE_EL1_NS
           | ICC_CTLR_EOI_MODE_EL1_S
           | ICC_CTLR_EOI_MODE_EL3_S;
  icc_ctrl &= ~(ICC_CTLR_CBPR_EL1_NS | ICC_CTLR_CBPR_EL1_S);
  sys_reg_write(ICC_CTLR_EL3, icc_ctrl);

  // for now we run at EL3, but access to EL1
  // and other lower EL levels must be set at
  // boot code (ACTLR_EL3 register)
  uint32_t icc_sre = sys_reg_read(ICC_SRE_EL3);
  if (icc_sre & ICC_SRE_SRE_ENABLE) {
    icc_sre = ICC_SRE_SRE_ENABLE
            | ICC_SRE_DFB_ENABLE
            | ICC_SRE_DIB_ENABLE
            | ICC_SRE_LOW_EL_ENABLE;
    sys_reg_read(ICC_SRE_EL3);
  }
  
  // enable group0 and group1 interrupts
  sys_reg_bit_set(ICC_IGRPEN0_EL1, 1);
  sys_reg_bit_set(ICC_IGRPEN1_EL1, 1);
}

void gic_v3_init(void)
{
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
		mmio_bit_set_u32(GICD_ISPENDR(GICD_BASE_ADDRESS, (int_id >> 5)),
			       (1u << (int_id % 32)));
	} else {
		uintptr_t base = gicr_rdist_base_addr();
		mmio_bit_set_u32(GICR_ISPENDR(gicr_rdist_base_addr(), 0), (1u << int_id));
	}
}

void gic_v3_int_clear_pending(unsigned int int_id)
{
	if (GIC_IS_SPI(int_id)) {
		mmio_bit_set_u32(GICD_ICPENDR(GICD_BASE_ADDRESS, (int_id >> 5)),
			       (1u << (int_id % 32)));
	} else {
		uintptr_t base = gicr_rdist_base_addr();
		mmio_bit_set_u32(GICR_ICPENDR(base, 0), (1u << int_id));
	}
}

void gic_v3_int_set_priority(unsigned int int_id, uint8_t prio)
{
	uintptr_t base = gicr_rdist_base_addr();

	if (GIC_IS_SPI(int_id)) {
		mmio_write_u32(GICD_IPRIORITYR(base, int_id), prio);
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
