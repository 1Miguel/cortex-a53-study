/**
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
 */

#ifndef MMIO__H
#define MMIO__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup mmio api's
 * @{
 * @ingroup mmio
 *
 * @brief MMIO api's
 */

#define __MMIO_FUNC_UINT(_size) \
  static inline void mmio_write_u##_size(uintptr_t addr, const uint##_size##_t val)\
  { \
    *(volatile uint##_size##_t *)addr = val; \
  } \
  static inline uint##_size##_t mmio_read_u##_size(uintptr_t addr) \
  { \
    return *(volatile uint##_size##_t *)addr; \
  } \
  static inline uint##_size##_t mmio_bit_get_u##_size(uintptr_t addr, const uint##_size##_t msk) \
  { \
    return *(volatile uint##_size##_t *)addr & msk; \
  } \
  static inline void mmio_bit_set_u##_size(uintptr_t addr, const uint##_size##_t msk) \
  { \
    *(volatile uint##_size##_t *)addr |= msk; \
  } \
  static inline void mmio_bit_clr_u##_size(uintptr_t addr, const uint##_size##_t msk) \
  { \
    *(volatile uint##_size##_t *)addr &= ~msk; \
  } \
  static inline void mmio_bit_write_u##_size(uintptr_t addr, const uint##_size##_t val, \
      const uint8_t pos, const uint8_t nbits) \
  { \
    uint32_t mem = mmio_read_u##_size(addr); \
    const uint32_t msk = (nbits == _size) ? UINT##_size##_MAX : (((1U << nbits) - 1U) << pos); \
    mmio_write_u##_size(addr, (mem & (~msk)) | ((val << pos) & msk)); \
  } \
  static inline uint##_size##_t mmio_bit_read_u##_size(uintptr_t addr, const uint8_t pos, \
      const uint8_t nbits) \
  { \
    uint32_t mem = mmio_read_u##_size(addr); \
    const uint32_t msk = (nbits == _size) ? UINT##_size##_MAX \
                      : ((1U << nbits) - 1U); \
    return ((mem >> pos) & msk); \
  }

__MMIO_FUNC_UINT(8)
__MMIO_FUNC_UINT(16)
__MMIO_FUNC_UINT(32)

#ifdef __cplusplus
}
#endif

/** @} */

#endif // MMIO__H
