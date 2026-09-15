#include <errno.h>
#include "uart.h"

int uart_pl011_init(struct uart_pl011 *uart, struct uart_pl011_cfg *cfg)
{
	if (!uart || !cfg || !uart->p_reg) {
		return -EINVAL;
	}
	uart->p_reg->LCR_H = 0;

	uart->p_reg->LCR_H |= (cfg->data_bits << UART_PL011_LCR_H_WLEN_BIT_POS);
	// enable fifo
	uart->p_reg->LCR_H |= UART_PL011_LCR_H_FEN_BIT_MSK;

	if (cfg->stop_bits) {
		uart->p_reg->LCR_H |= UART_PL011_LCR_H_STP2_BIT_MSK;
	}

	if (cfg->parity) {
		uart->p_reg->LCR_H |= UART_PL011_LCR_H_PEN_BIT_MSK;
	}

  return 0;
}

int uart_pl011_enable(struct uart_pl011 *uart)
{
	if (!uart) {
		return -EINVAL;
	}
	uart->p_reg->CR |= (UART_PL011_CR_UARTEN_BIT_MSK | UART_PL011_CR_TXE_BIT_MSK |
			    UART_PL011_CR_RXE_BIT_MSK);

	return 0;
}

void uart_pl011_poll_out(struct uart_pl011 *uart, uint8_t c)
{
	uart->p_reg->DR = c;
}

char uart_pl011_poll_in(struct uart_pl011 *uart)
{
	while (uart->p_reg->FR & UART_PL011_FR_RXFE_BIT_MSK)
		;
	return uart->p_reg->DR;
}
