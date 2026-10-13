/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

#ifndef MMIO__H
#define MMIO__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mmio api's
 * @{
 *
 * @ingroup mmio
 *
 * @brief MMIO api's
 */

/**
 * @brief A convenient macro for creating mmio accessor functions.
 *
 * @param _size Unsigned bit size (8, 16, 32, 64).
 */
#define __MMIO_FUNC_UINT(_size)                                                                    \
	static inline void mmio_write_u##_size(uintptr_t addr, const uint##_size##_t val)          \
	{                                                                                          \
		*(volatile uint##_size##_t *)addr = val;                                           \
	}                                                                                          \
	static inline uint##_size##_t mmio_read_u##_size(uintptr_t addr)                           \
	{                                                                                          \
		return *(volatile uint##_size##_t *)addr;                                          \
	}                                                                                          \
	static inline uint##_size##_t mmio_bit_get_u##_size(uintptr_t addr,                        \
							    const uint##_size##_t msk)             \
	{                                                                                          \
		return *(volatile uint##_size##_t *)addr & msk;                                    \
	}                                                                                          \
	static inline void mmio_bit_set_u##_size(uintptr_t addr, const uint##_size##_t msk)        \
	{                                                                                          \
		*(volatile uint##_size##_t *)addr |= msk;                                          \
	}                                                                                          \
	static inline void mmio_bit_clr_u##_size(uintptr_t addr, const uint##_size##_t msk)        \
	{                                                                                          \
		*(volatile uint##_size##_t *)addr &= ~msk;                                         \
	}                                                                                          \
	static inline void mmio_bit_write_u##_size(uintptr_t addr, const uint##_size##_t val,      \
						   const uint8_t pos, const uint8_t nbits)         \
	{                                                                                          \
		uint##_size##_t mem = mmio_read_u##_size(addr);                                    \
		const uint##_size##_t msk =                                                        \
			(nbits == _size) ? UINT##_size##_MAX : (((1U << nbits) - 1U) << pos);      \
		mmio_write_u##_size(addr, (mem & (~msk)) | ((val << pos) & msk));                  \
	}                                                                                          \
	static inline uint##_size##_t mmio_bit_read_u##_size(uintptr_t addr, const uint8_t pos,    \
							     const uint8_t nbits)                  \
	{                                                                                          \
		uint##_size##_t mem = mmio_read_u##_size(addr);                                    \
		const uint##_size##_t msk =                                                        \
			(nbits == _size) ? UINT##_size##_MAX : ((1U << nbits) - 1U);               \
		return ((mem >> pos) & msk);                                                       \
	}

__MMIO_FUNC_UINT(8)
__MMIO_FUNC_UINT(16)
__MMIO_FUNC_UINT(32)
__MMIO_FUNC_UINT(64)

#ifdef __cplusplus
}
#endif

/** @} */

#endif // MMIO__H
