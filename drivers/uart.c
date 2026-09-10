#include "uart.h"
#include "uart_pl011_regs.h"

int uart_pl011_init(struct uart_pl011 *uart, struct uart_pl011_cfg *cfg)
{
	volatile struct uart_pl011_regs *p_reg;

	if (!uart || !cfg || !uart->p_reg) {
		return -EINVAL;
	}

	p_reg = (volatile struct uart_pl011_regs *)uart->p_reg;

	p_reg->LCR_H |= (cfg->data_bits << UART_PL011_LCR_H_WLEN_BIT_POS);
	p_reg->LCR_H |= UART_PL011_LCR_H_FEN_BIT_MSK;
}

int uart_pl011_enable(struct uart_pl011 *uart)
{
	if (!uart) {
		return->EINVAL;
	}
	p_reg->CR |= (UART_PL011_CR_UARTEN_BIT_MSK | UART_PL011_CR_TXE_BIT_MSK |
		      UART_PL011_CR_RXE_BIT_MSK);

	return 0;
}

int uart_pl011_poll_in(struct uart_pl011 *uart, uint8_t c)
{
	p_reg->DR = c;
}

void uart_pl011_poll_out(struct uart_pl011 *uart)
{
}
