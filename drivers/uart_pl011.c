#include <errno.h>
#include "uart.h"
#include "uart_pl011_regs.h"

void uart_irq_handler(void)
{
}

int uart_init(struct uart *uart, struct uart_cfg *cfg)
{
	if (!uart || !cfg || !uart->p_reg) {
		return -EINVAL;
	}

	struct uart_pl011_regs *p_reg = (struct uart_pl011_regs *)uart->p_reg;

	p_reg->LCR_H = 0;
	p_reg->LCR_H |= (cfg->data_bits << UART_PL011_LCR_H_WLEN_BIT_POS);

	// enable fifo
	p_reg->LCR_H |= UART_PL011_LCR_H_FEN_BIT_MSK;

	if (cfg->stop_bits) {
		p_reg->LCR_H |= UART_PL011_LCR_H_STP2_BIT_MSK;
	}

	if (cfg->parity) {
		p_reg->LCR_H |= UART_PL011_LCR_H_PEN_BIT_MSK;
	}

	return 0;
}

int uart_enable(struct uart *uart)
{
	if (!uart || !uart->p_reg) {
		return -EINVAL;
	}

	struct uart_pl011_regs *p_reg = (struct uart_pl011_regs *)uart->p_reg;

	p_reg->CR |= (UART_PL011_CR_UARTEN_BIT_MSK | UART_PL011_CR_TXE_BIT_MSK |
		      UART_PL011_CR_RXE_BIT_MSK);

	return 0;
}

void uart_poll_out(struct uart *uart, uint8_t c)
{
	if (!uart || !uart->p_reg) {
		return;
	}

	struct uart_pl011_regs *p_reg = (struct uart_pl011_regs *)uart->p_reg;

	p_reg->DR = c;
}

char uart_poll_in(struct uart *uart)
{
	if (!uart || !uart->p_reg) {
		return 0;
	}

	struct uart_pl011_regs *p_reg = (struct uart_pl011_regs *)uart->p_reg;

	while (p_reg->FR & UART_PL011_FR_RXFE_BIT_MSK)
		;
	return p_reg->DR;
}
