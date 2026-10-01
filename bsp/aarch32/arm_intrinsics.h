/**
 * @file arm_intrinsics.h
 * @brief Intrinsics macros or functions.
 */
#ifndef BSP_AARCH32_ARM_H
#define BSP_AARCH32_ARM_H

#include <stdint.h>

/**
 * @brief Write a general-purpose register value to a coprocessor register.
 *
 * The coprocessor and register fields must be compile-time integer literals.
 */
#define __MCR(cp, opc1, val, crn, crm, opc2)                                                       \
	do {                                                                                       \
		uint32_t _val = (uint32_t)(val);                                                   \
		__asm__ volatile("mcr p" #cp ", " #opc1 ", %0, c" #crn ", c" #crm ", " #opc2       \
				 :                                                                 \
				 : "r"(_val)                                                       \
				 : "memory");                                                      \
	} while (0)

/**
 * @brief Read a coprocessor register into a general-purpose register value.
 *
 * The coprocessor and register fields must be compile-time integer literals.
 */
#define __MRC(cp, opc1, val, crn, crm, opc2)                                                       \
	do {                                                                                       \
		uint32_t _val;                                                                     \
		__asm__ volatile("mrc p" #cp ", " #opc1 ", %0, c" #crn ", c" #crm ", " #opc2       \
				 : "=r"(_val)                                                      \
				 :                                                                 \
				 : "memory");                                                      \
		(val) = _val;                                                                      \
	} while (0)

#endif /* BSP_AARCH32_ARM_H */
