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

static void fifo_flush(struct uart_pl011 *uart)
{
	mmio_bit_clr_u16(UARTLCR_H(uart->reg), UART_PL011_LCR_H_FEN_BIT_MSK);
	mmio_bit_set_u16(UARTLCR_H(uart->reg), UART_PL011_LCR_H_FEN_BIT_MSK);
}

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

int uart_pl011_set_irq_cb(struct uart_pl011 *uart, uart_pl011_irq_cb_t cb, void *cb_data)
{
	if (!uart) {
		return -EINVAL;
	}
	uart->irq_cb = cb;
	uart->irq_cb_data = cb_data;
}

int uart_pl011_enable(struct uart_pl011 *uart)
{
	if (!uart) {
		return -EINVAL;
	}

	fifo_flush(uart);

	mmio_bit_set_u32(UARTCR(uart->reg),
			 (UART_PL011_CR_UARTEN_BIT_MSK | UART_PL011_CR_TXE_BIT_MSK |
			  UART_PL011_CR_RXE_BIT_MSK));

	return 0;
}

int uart_pl011_poll_out(struct uart_pl011 *uart, uint8_t c)
{
	if (!uart) {
		return -EINVAL;
	}
	mmio_write_u32(UARTDR(uart->reg), (uint32_t)c);
	while (!mmio_bit_get_u32(UARTFR(uart->reg), UART_PL011_FR_TXFE_BIT_MSK))
		;
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

int uart_pl011_rx_fifo_ready(struct uart_pl011 *uart)
{
	uint16_t fr;
	if (!uart) {
		return -EINVAL;
	}
	fr = mmio_bit_get_u32(UARTFR(uart->reg), UART_PL011_FR_RXFE_BIT_MSK);
	return fr ? 0 : 1;
}

int uart_pl011_tx_fifo(struct uart_pl011 *uart, uint8_t *buf, uint16_t len)
{
	int n = 0;
	do {
		if (mmio_bit_get_u16(UARTFR(uart->reg), UART_PL011_FR_TXFF_BIT_MSK)) {
			break;
		}
		mmio_write_u32(UARTDR(uart->reg), *(buf + n));
		n++;
	} while (len--);
	return n;
}

int uart_pl011_rx_fifo(struct uart_pl011 *uart, uint8_t *buf, uint16_t len)
{
	int n = 0;
	do {
		if (mmio_bit_get_u16(UARTFR(uart->reg), UART_PL011_FR_RXFE_BIT_MSK)) {
			break;
		}
		*(buf + n) = mmio_read_u32(UARTDR(uart->reg));
		n++;
	} while (len--);
	return n;
}

int uart_pl011_err_chk(struct uart_pl011 *uart)
{
	if (!uart) {
		return -EINVAL;
	}
	return (int)mmio_read_u16(UARTRIS(uart->reg));
}

int uart_pl011_tx_irq_enable(struct uart_pl011 *uart)
{
	mmio_bit_set_u16(UARTIMSC(uart->reg), UART_PL011_IMSC_TXIM_BIT_MSK);
}

int uart_pl011_tx_irq_disable(struct uart_pl011 *uart)
{
	mmio_bit_clr_u16(UARTIMSC(uart->reg), UART_PL011_IMSC_TXIM_BIT_MSK);
}

int uart_pl011_rx_irq_enable(struct uart_pl011 *uart)
{
	mmio_bit_set_u16(UARTIMSC(uart->reg),
			 UART_PL011_IMSC_RXIM_BIT_MSK | UART_PL011_IMSC_RTIM_BIT_MSK);
}

int uart_pl011_rx_irq_disable(struct uart_pl011 *uart)
{
	mmio_bit_clr_u16(UARTIMSC(uart->reg),
			 UART_PL011_IMSC_RXIM_BIT_MSK | UART_PL011_IMSC_RTIM_BIT_MSK);
}

int uart_pl011_err_irq_enable(struct uart_pl011 *uart)
{
	mmio_bit_set_u16(UARTIMSC(uart->reg), UART_PL011_IMSC_PEIM_BIT_MSK |
						      UART_PL011_IMSC_BEIM_BIT_MSK |
						      UART_PL011_IMSC_OEIM_BIT_MSK);
}

int uart_pl011_err_irq_disable(struct uart_pl011 *uart)
{
	mmio_bit_clr_u16(UARTIMSC(uart->reg), UART_PL011_IMSC_PEIM_BIT_MSK |
						      UART_PL011_IMSC_BEIM_BIT_MSK |
						      UART_PL011_IMSC_OEIM_BIT_MSK);
}

void uart_pl011_irq_handler(struct uart_pl011 *uart)
{
	uint16_t uartmis;

	if (!uart) {
		return;
	}

	uartmis = mmio_read_u16(UARTMIS(uart->reg));

	if (uartmis && uart->irq_cb) {
		uart->irq_cb(uart, uart->irq_cb_data);
	}

	mmio_bit_set_u16(UARTICR(uart->reg), uartmis);
}
