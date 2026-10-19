/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef TIMER__H
#define TIMER__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup gtimer api's
 * @{
 *
 * @ingroup gtimer
 *
 * @brief ARM generic timer.
 */

/**
 * @brief Timer timeout callback function type.
 *
 * @param[out] user callback data.
 *
 */
typedef void (*timer_irq_cb_t)(void *cb_data);

/**
 * @brief Set interrupt callback function and data.
 *
 * @param[in] irq_cb  interrupt callback function pointer.
 * @param[in] cb_data pointer to callback data to pass to function.
 *
 */
void timer_set_irq_cb(timer_irq_cb_t irq_cb, void *cb_data);

/**
 * @brief blocking poll until timeout.
 */
void timer_poll(void);

/**
 * @brief Start the generic timer.
 *
 */
void timer_start(void);

/**
 * @brief Stop the generic timer.
 *
 */
void timer_stop(void);

/**
 * @brief get current time.
 */
void timer_get_count(void);

/**
 * @brief Set timeout relative to current time count.
 *
 * @param cnt Relative timeout ticks until timeout.
 *
 */
void timer_set_relative(uint32_t cnt);

/**
 * @brief Set absolute timeout count.
 *
 * @param cnt Absolute tick to until timeout trigger.
 *
 */
void timer_set_absolute(uint64_t cnt);

uint64_t timer_ms_to_cnt(uint32_t ms);

uint32_t timer_cnt_to_ms(uint64_t ticks);

uint64_t timer_us_to_cnt(uint32_t us);

uint32_t timer_cnt_to_us(uint64_t cnt);

/** @} */

#endif // TIMER__H
