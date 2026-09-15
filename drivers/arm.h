/**
 * @file
 * @brief
 */
#ifndef __AARCH64_H
#define __AARCH64_H

#define __EA_BIT  (1U << 3)
#define __IRQ_BIT (1U << 1)
#define __FIQ_BIT (1U << 2)

/**
 * @brief
 *
 * @param reg
 *
 * @retval
 */
#define __MRS(reg)                                                                                 \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm__ volatile("mrs %0, " #reg : "=r"(__val));                                   \
		__val;                                                                             \
	})

/**
 * @brief
 *
 * @param reg
 * @param val
 *
 * @retval
 */
#define MSR(reg, val) __asm__ volatile("msr " STR(reg) ", %0" : : "r"(val) : "memory")

static void __aarch64_el3_irq_enable(void)
{
	__MRS(SCR_EL3, (1U << 1));
}

static void __aarch64_el3_irq_disable(void)
{
}

#endif // __AARCH64_H
