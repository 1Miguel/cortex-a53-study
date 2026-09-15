#ifndef UART_PL011__H
#define UART_PL011__H

#include <stdint.h>

#include "uart_pl011_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

enum uart_pl011_config_data_bits {
	UART_CFG_DATA_BITS_5 = 0U,
	UART_CFG_DATA_BITS_6 = 1U,
	UART_CFG_DATA_BITS_7 = 2U,
	UART_CFG_DATA_BITS_8 = 3U,
	UART_CFG_DATA_BITS_9 = 4U,
};

enum uart_pl011_config_stop_bits {
	UART_CFG_STOP_1_BIT = 0U,
	UART_CFG_STOP_2_BIT = 1U,
};

struct uart_pl011_cfg {
	uint8_t parity;
	uint8_t stop_bits;
	uint8_t data_bits;
	uint8_t flow_ctrl;
};

struct uart_pl011 {
	struct uart_pl011_regs *p_reg;
};

int uart_pl011_init(struct uart_pl011 *uart, struct uart_pl011_cfg *cfg);

int uart_pl011_enable(struct uart_pl011 *uart);

char uart_pl011_poll_in(struct uart_pl011 *uart);

void uart_pl011_poll_out(struct uart_pl011 *uart, uint8_t c);

#ifdef __cplusplus
}
#endif

#endif // UART_PL011__H
