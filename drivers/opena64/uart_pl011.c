/*******************************************************************************
 * @file uart_pl011.c
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#include <errno.h>
#include <stdint.h>
#include "mmio.h"
#include "uart_pl011.h"
#include "uart_pl011_regs.h"

int uart_pl011_init(struct uart_pl011 *uart, struct uart_pl011_cfg *cfg)
{
	if (!uart || !cfg) {
		return -EINVAL;
	}

	uintptr_t lcr_h = 0;

	lcr_h |= (cfg->data_bits << UART_PL011_LCR_H_WLEN_BIT_POS);

	// enable fifo
	lcr_h |= UART_PL011_LCR_H_FEN_BIT_MSK;

	if (cfg->stop_bits) {
		lcr_h |= UART_PL011_LCR_H_STP2_BIT_MSK;
	}
	if (cfg->parity) {
		lcr_h |= UART_PL011_LCR_H_PEN_BIT_MSK;
	}

	mmio_write_u32(UARTLCR_H(uart->reg), lcr_h);

	return 0;
}

int uart_pl011_enable(struct uart_pl011 *uart)
{
	if (!uart) {
		return -EINVAL;
	}
	mmio_bit_set_u32(UARTCR(uart->reg),
			 (UART_PL011_CR_UARTEN_BIT_MSK | UART_PL011_CR_TXE_BIT_MSK |
			  UART_PL011_CR_RXE_BIT_MSK));

	return 0;
}

void uart_pl011_poll_out(struct uart_pl011 *uart, uint8_t c)
{
	if (!uart) {
		return;
	}
	mmio_write_u32(UARTDR(uart->reg), (uint32_t)c);
}

char uart_pl011_poll_in(struct uart_pl011 *uart)
{
	if (!uart) {
		return 0;
	}
	while (mmio_bit_get_u32(UARTFR(uart->reg), UART_PL011_FR_RXFE_BIT_MSK))
		;

	return (char)mmio_read_u32(UARTDR(uart->reg));
}
