/*******************************************************************************
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/

typedef void (*irq_handler_t)(void);

__attribute__((weak)) void el3_cntp_irq_handler(void)
{
	while (1)
		;
}

__attribute__((weak)) void el1_cntp_irq_handler(void)
{
	while (1)
		;
}

__attribute__((weak)) void el1_cntv_irq_handler(void)
{
	while (1)
		;
}

__attribute__((weak)) void int_33_irq_handler(void)
{
	while (1)
		;
}

__attribute__((weak)) void int_34_irq_handler(void)
{
	while (1)
		;
}

const irq_handler_t __gic_irq_vec_tbl[] __attribute__((section(".gic_vec_tbl"))) = {
	// software generate interrupts (SGI)
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	// private peripheral interrupts (PPI)
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	el1_cntv_irq_handler,
	0,
	el3_cntp_irq_handler,
	el1_cntp_irq_handler,
	0,
	// shared peripheral interrupt ids
	0,
	int_33_irq_handler,
	int_34_irq_handler,
};
