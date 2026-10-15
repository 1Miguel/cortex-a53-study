/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#include <string.h>
#include "arm64.h"
#include "timer.h"

static timer_irq_cb_t m_irq_cb = NULL;
static timer_irq_cb_t m_cb_data = NULL;

static inline void irq_enable(void)
{
	sys_reg_bit_clr(CNTP_CTL_EL0, 2u);
}

static inline void irq_disable(void)
{
	sys_reg_bit_set(CNTP_CTL_EL0, 2u);
}

void timer_set_irq_cb(arm_gen_timer_irq_cb_t irq_cb, void *cb_data)
{
	m_irq_cb = irq_cb;
	m_cb_data = cb_data;
}

void timer_start(void)
{
	sys_reg_bit_set(CNTP_CTL_EL0, 1u);
}

void timer_stop(void)
{
	sys_reg_bit_clr(CNTP_CTL_EL0, 1u);
}

void timer_poll(void)
{
	volatile uint64_t status;
	do {
		status = sys_reg_read(CNTP_CTL_EL0);
	} while (!(status & (1ULL << 2)));
}

void timer_set_relative(uint32_t tick)
{
	sys_reg_write(CNTP_TVAL_EL0, (uint64_t)tick);
  if (m_irq_cb) {
	  irq_enable();
  }
}

void timer_set_absolute(uint64_t tick)
{
	sys_reg_write(CNTP_CVAL_EL0, tick);
  if (m_irq_cb) {
	  irq_enable();
  }
}

uint64_t timer_ms_to_cnt(uint32_t ms)
{
	return ((uint64_t)ms * sys_reg_read(CNTFRQ_EL0)) / 1000ULL;
}

uint32_t timer_cnt_to_ms(uint64_t cnt)
{
	return (uint32_t)((cnt * 1000ULL) / sys_reg_read(CNTFRQ_EL0));
}

uint64_t timer_us_to_cnt(uint32_t us)
{
	return ((uint64_t)us * sys_reg_read(CNTFRQ_EL0)) / 1000000ULL;
}

uint32_t timer_cnt_to_us(uint64_t cnt)
{
	return (uint32_t)((cnt * 1000000ULL) / sys_reg_read(CNTFRQ_EL0));
}

void el1_cntp_irq_handler(void)
{
  irq_disable();
	if (m_irq_cb) {
		m_irq_cb(m_cb_data);
	}
}
