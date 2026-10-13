/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#include <string.h>
#include "arm64.h"
#include "arm_gen_timer.h"

static arm_gen_timer_irq_cb_t *m_irq_cb = NULL;
static arm_gen_timer_irq_cb_t *m_cb_data = NULL;

void arm_gen_timer_init(arm_gen_timer_irq_cb_t *irq_cb, void *cb_data)
{
	m_irq_cb = irq_cb;
	m_cb_data = cb_data;
}

void arm_gen_timer_start(void)
{
	sys_reg_bit_set(CNTP_CTL_EL0, 1u);
}

void arm_gen_timer_stop(void)
{
	sys_reg_bit_clr(CNTP_CTL_EL0, 1u);
}

void arm_gen_irq_enable(void)
{
	sys_reg_bit_clr(CNTP_CTL_EL0, 2u);
}

void arm_gen_timer_poll(void)
{
	volatile uint64_t status;
	do {
		status = sys_reg_read(CNTP_CTL_EL0);
	} while (!(status & (1ULL << 2)));
}

void arm_gen_timer_set_relative(uint32_t tick)
{
	sys_reg_write(CNTP_TVAL_EL0, (uint64_t)tick);
}

void arm_gen_timer_set_absolute(uint64_t tick)
{
	sys_reg_write(CNTP_CVAL_EL0, tick);
}

uint64_t arm_gen_timer_ms_to_cnt(uint32_t ms)
{
	return ((uint64_t)ms * sys_reg_read(CNTFRQ_EL0)) / 1000ULL;
}

uint32_t arm_gen_timer_cnt_to_ms(uint64_t cnt)
{
	return (uint32_t)((cnt * 1000ULL) / sys_reg_read(CNTFRQ_EL0));
}

uint64_t arm_gen_timer_us_to_cnt(uint32_t us)
{
	return ((uint64_t)us * sys_reg_read(CNTFRQ_EL0)) / 1000000ULL;
}

uint32_t arm_gen_timer_cnt_to_us(uint64_t cnt)
{
	return (uint32_t)((cnt * 1000000ULL) / sys_reg_read(CNTFRQ_EL0));
}
