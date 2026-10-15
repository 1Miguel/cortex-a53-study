/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef __GIC__H
#define __GIC__H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Initializes the distributor, current PE redistributor, and CPU interface.
 */
void gic_v3_init(void);

/**
 * @brief Enable interrupt.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_enable(unsigned int int_id);

/**
 * @brief Disable interrupt.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_disable(unsigned int int_id);

/**
 * @brief Check if a given interrupt id is enabled.
 *
 * @param int_id interrupt id.
 *
 * @retval true if interrupt is enabled.
 */
bool gic_v3_int_is_enable(unsigned int int_id);

/**
 * @brief Check if a given interrupt id is pending.
 *
 * @param int_id interrupt id.
 *
 * @retval true if interrupt is pending.
 */
bool gic_v3_int_is_pending(unsigned int int_id);

/**
 * @brief Set pending interrupt of a given interrupt id.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_set_pending(unsigned int int_id);

/**
 * @brief Clear any pending interrupt of a given interrupt id.
 *
 * @param int_id interrupt id.
 */
void gic_v3_int_clear_pending(unsigned int int_id);

/**
 * @brief Sets an interrupt's priority.
 *
 * @param int_id GIC interrupt ID.
 * @param priority 8-bit GIC priority value.
 * @return False when the driver is not initialized or the ID is unsupported.
 */
void gic_v3_int_set_priority(unsigned int int_id, uint8_t priority);

/**
 * @brief Sends an SGI to the selected Aff0 targets.
 *
 * @param sgi_id SGI ID in the range 0-15.
 * @param target_aff Target MPIDR affinity value; Aff0[7:4] selects the range.
 * @param target_list Bit mask selecting Aff0[3:0] targets in the affinity group.
 * @return False when the driver is not initialized or the arguments are invalid.
 */
void gic_v3_raise_sgi(unsigned int sgi_id, uint64_t target_aff, uint16_t target_list);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __GIC__H
