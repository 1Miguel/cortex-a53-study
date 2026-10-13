/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef GTIMER__H
#define GTIMER__H

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
typedef void (*arm_gen_timer_irq_cb_t)(void *cb_data);

/**
 * @brief Initialise the timer.
 *
 * @param[in] irq_cb  interrupt callback function pointer.
 * @param[in] cb_data pointer to callback data to pass to function.
 *
 */
void arm_gen_timer_init(arm_gen_timer_irq_cb_t *irq_cb, void *cb_data);

/**
 * @brief IRQ interrupt enable.
 *
 */
void arm_gen_timer_irq_enable(void);

/**
 * @brief blocking poll until timeout.
 */
void arm_gen_timer_poll(void);

/**
 * @brief Start the generic timer.
 *
 */
void arm_gen_timer_start(void);

/**
 * @brief Stop the generic timer.
 *
 */
void arm_gen_timer_stop(void);

/**
 * @brief get current time.
 */
void arm_gen_timer_get_count(void);

/**
 * @brief Set timeout relative to current time count.
 *
 * @param cnt Relative timeout ticks until timeout.
 *
 */
void arm_gen_timer_set_relative(uint32_t cnt);

/**
 * @brief Set absolute timeout count.
 *
 * @param cnt Absolute tick to until timeout trigger.
 *
 */
void arm_gen_timer_set_absolute(uint64_t cnt);

uint64_t arm_gen_timer_ms_to_cnt(uint32_t ms);

uint32_t arm_gen_timer_cnt_to_ms(uint64_t ticks);

uint64_t arm_gen_timer_us_to_cnt(uint32_t us);

uint32_t arm_gen_timer_cnt_to_us(uint64_t cnt);

/** @} */

#endif // GTIMER__H
