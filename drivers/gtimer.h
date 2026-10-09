/*******************************************************************************
 * @file gtimer.c
 * @brief ARM generic timer.
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
#ifndef GTIMER__H
#define GTIMER__H

#include "uart_pl011_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Timer timeout callback function type.
 *
 * @param[out] user callback data.
 *
 */
typedef void (*gtimer_irq_cb_t)(void *user_data);

/**
 * @brief Enable the generic timer.
 *
 */
void gtimer_enable(void);

/**
 * @brief Pause the generic timer.
 *
 */
void gtimer_pause(void);

/**
 * @brief Set timeout relative to current time tick.
 *
 * @param tick Relative timeout ticks until timeout.
 *
 */
void gtimer_set_rel_timeout(uint32_t tick);

/**
 * @brief Set absolute timeout tick.
 *
 * @param tick Absolute tick to until timeout trigger.
 *
 */
void gtimer_set_abs_timeout(uint64_t tick);

#endif // GTIMER__H
