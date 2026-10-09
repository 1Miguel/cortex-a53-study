/*******************************************************************************
 * @file arm.h
 * @brief Common ARM instruction helpers.
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
#ifndef __ARM__H
#define __ARM__H

#include "arm64.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Executes a no-operation instruction.
 */
static inline void nop(void)
{
	__asm__ volatile("nop");
}

/**
 * @brief Gives a hint that the processor can yield to another thread.
 */
static inline void yield(void)
{
	__asm__ volatile("yield" ::: "memory");
}

/**
 * @brief Waits for an event.
 */
static inline void wfe(void)
{
	__asm__ volatile("wfe" ::: "memory");
}

/**
 * @brief Waits for an interrupt.
 */
static inline void wfi(void)
{
	__asm__ volatile("wfi" ::: "memory");
}

/**
 * @brief Sends an event to other processing elements.
 */
static inline void sev(void)
{
	__asm__ volatile("sev" ::: "memory");
}

/**
 * @brief Clears the local exclusive monitor.
 */
static inline void clrex(void)
{
	__asm__ volatile("clrex" ::: "memory");
}

/**
 * @brief Executes a full-system data memory barrier.
 */
static inline void dmb(void)
{
	__asm__ volatile("dmb sy" ::: "memory");
}

/**
 * @brief Orders data accesses in the inner-shareable domain.
 */
static inline void dmb_ish(void)
{
	__asm__ volatile("dmb ish" ::: "memory");
}

/**
 * @brief Orders data stores in the inner-shareable domain.
 */
static inline void dmb_ishst(void)
{
	__asm__ volatile("dmb ishst" ::: "memory");
}

/**
 * @brief Completes all prior data accesses in the full system.
 */
static inline void dsb(void)
{
	__asm__ volatile("dsb sy" ::: "memory");
}

/**
 * @brief Completes all prior data accesses in the full system.
 */
static inline void dsb_sy(void)
{
	__asm__ volatile("dsb sy" ::: "memory");
}

/**
 * @brief Completes all prior data accesses in the inner-shareable domain.
 */
static inline void dsb_ish(void)
{
	__asm__ volatile("dsb ish" ::: "memory");
}

/**
 * @brief Synchronizes the instruction stream with prior system changes.
 */
static inline void isb(void)
{
	__asm__ volatile("isb" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* __ARM__H */
