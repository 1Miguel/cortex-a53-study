/**
 * @file arm.h
 * @brief aarch64 system functions that allows c appcode to
 *    directly call aarh64 instructions.
 */

#ifndef __AARCH64_H
#define __AARCH64_H

#include <stdint.h>

#define __EA_BIT  (1U << 3)
#define __IRQ_BIT (1U << 1)
#define __FIQ_BIT (1U << 2)

/**
 * @brief Move System Register allows the PE to read an AArch64 System
 *    register into a general-purpose register.
 *
 * @param reg System Register name.
 *
 * @return System Register Value.
 */
#define __MRS(reg)                                                                                 \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm__ volatile("mrs %0, " #reg : "=r"(__val));                                   \
		__val;                                                                             \
	})

/**
 * @brief Move general-purpose register to System Register allows the PE to
 *    write an AArch64 System register from a general-purpose register.
 *
 * @param reg System Register name.
 * @param val Value to write.
 */
#define __MSR(reg, val) __asm__ volatile("msr " #reg ", %0" : : "r"(val) : "memory")

#define smc(imm) \
    __asm__ volatile("smc #" #imm ::: "memory")

static inline void dsb_sy(void)
{
  __asm__ volatile("dsb sy" ::: "memory");
}

static inline void isb(void)
{
  __asm__ volatile("isb" ::: "memory");
}

static inline void el3_irq_enable(void)
{
	__MSR(SCR_EL3, __IRQ_BIT);
}

static inline void el3_irq_disable(void)
{
  uint64_t reg = __MRS(SCR_EL3);
	__MSR(SCR_EL3, reg & ((uint64_t)~__IRQ_BIT));
}

#endif // __AARCH64_H
