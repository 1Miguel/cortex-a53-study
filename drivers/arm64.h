/*******************************************************************************
 * @file arm64.h
 * @brief AArch64 system-register and instruction helpers for C code.
 *
 * MIT License
 *
 * Copyright (c) 2026 Miguel Ugsimar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *******************************************************************************/
#ifndef __AARCH64__H
#define __AARCH64__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup arm64 arch specific instruction api's
 * @{
 * @ingroup arm64
 *
 * @brief MMIO api's
 */

#define EA_BIT  (1U << 3)
#define IRQ_BIT (1U << 1)
#define FIQ_BIT (1U << 2)

/**
 * @brief Reads an AArch64 system register.
 *
 * @param reg System register name.
 *
 * @return Value read from the system register.
 */
#define __MRS(reg)                                                                                 \
	({                                                                                         \
		uint64_t __val;                                                                    \
		__asm__ volatile("mrs %0, " #reg : "=r"(__val));                                   \
		__val;                                                                             \
	})

/**
 * @brief Writes a value to an AArch64 system register.
 *
 * @param reg System register name.
 * @param val Value to write.
 */
#define __MSR(reg, val) \
  { \
    __asm__ volatile("msr " #reg ", %0" : : "r"(val) : "memory"); \
  }

/**
 * @brief Executes a Secure Monitor Call with an immediate value.
 *
 * @param imm Immediate value passed to the SMC instruction.
 */
#define smc(imm) \
  { \
    __asm__ volatile("smc #" #imm::: "memory"); \
  }

#define __SYS_REG_FUNC(reg) \
  static inline void sys_reg_##reg##_write(uint64_t val) \
  { \
    __MSR(reg, val); \
  } \
\
  static inline uint64_t sys_reg_##reg##_read(void) \
  { \
    return __MRS(reg); \
  } \
\
  static inline void sys_reg_##reg##_bit_set(uint64_t bit_msk) \
  { \
    sys_reg_##reg##_write(sys_reg_##reg##_read() | bit_msk); \
  } \
\
  static inline void sys_reg_##reg##_bit_clr(uint64_t bit_msk) \
  { \
    sys_reg_##reg##_write(sys_reg_##reg##_read() & (~bit_msk)); \
  } \
\
  static inline uint64_t sys_reg_##reg##_bit_get(uint64_t bit_msk) \
  { \
    return sys_reg_##reg##_read() & bit_msk; \
  }

__SYS_REG_FUNC(currentEL)
__SYS_REG_FUNC(SCR_EL3)
__SYS_REG_FUNC(MPIDR_EL3)
__SYS_REG_FUNC(SCR_EL1)
__SYS_REG_FUNC(MPIDR_EL1)
__SYS_REG_FUNC(ICC_IAR0_EL1)
__SYS_REG_FUNC(ICC_EOIR_EL1)
__SYS_REG_FUNC(ICC_BPR0_EL1)
__SYS_REG_FUNC(ICC_SRE_EL1)
__SYS_REG_FUNC(ICC_SGI0R_EL1)
__SYS_REG_FUNC(ICC_IGRPEN0_EL1)
__SYS_REG_FUNC(ICC_IGRPEN1_EL1)
__SYS_REG_FUNC(ICC_PMR_EL1)

__SYS_REG_FUNC(ICC_CTLR_EL3)
__SYS_REG_FUNC(ICC_SRE_EL3)

/**
 * @brief helper function to read arm system register.
 *
 * @param reg name of system register.
 */
#define sys_reg_read(reg) sys_reg_##reg##_read()

/**
 * @brief helper function to write to an arm system register.
 *
 * @param reg name of system register.
 * @param val  value to write
 */
#define sys_reg_write(reg, val) sys_reg_##reg##_write(val)

/**
 * @brief helper function to set arm system register bit field.
 *
 * @param reg     name of system register.
 * @param bit_msk bit mask selecting the bits to set.
 */
#define sys_reg_bit_set(reg, bit_msk) sys_reg_##reg##_bit_set(bit_msk)

/**
 * @brief helper function to set arm system register bit field.
 *
 * @param reg     name of system register.
 * @param bit_msk bit mask selecting the bits to clear.
 */
#define sys_reg_bit_clr(reg, bit_msk) sys_reg_##reg##_bit_clr(bit_msk)

/**
 * @brief helper function to set arm system register bit field.
 *
 * @param reg     name of system register.
 * @param bit_msk bit mask selecting the bits to get.
 */
#define sys_reg_bit_get(reg, bit_msk) sys_reg_##reg##_bit_get(bit_msk)

/**
 * @brief Enables IRQ routing to EL3 by setting the SCR_EL3.IRQ bit.
 */
static inline void irq_enable_el3(void)
{
	sys_reg_bit_set(SCR_EL3, IRQ_BIT);
}

/**
 * @brief Disables IRQ routing to EL3 by clearing the SCR_EL3.IRQ bit.
 */
static inline void irq_disable_el3(void)
{
	sys_reg_bit_clr(SCR_EL3, IRQ_BIT);
}

/**
 * @brief Get the current cpu id.
 *
 * @return Current cpu id.
 */
static inline uint8_t curr_cpu_id(void)
{
	return sys_reg_bit_get(MPIDR_EL1, 0xFF);
}

/**
 * @brief Get the current exception level (EL).
 *
 * @return Current exception level.
 */
static inline uint8_t curr_exc_lvl(void)
{
  uint64_t curr_el = sys_reg_read(currentEL);
	return curr_el >> 2;
}

#ifdef __cplusplus
}
#endif

/** @} */

#endif // __AARCH64__H
