/*******************************************************************************
 * @file uart_pl011.h
 * @brief High level PL011 UART driver.
 * @author Miguel Ugsimar <jmjugsimar@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 *******************************************************************************/
#ifndef UART_PL011__H
#define UART_PL011__H

#include "uart_pl011_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief uart pl011 number of data bits enum options.
 *
 */
enum uart_pl011_config_data_bits {
	UART_CFG_DATA_BITS_5 = 0U,
	UART_CFG_DATA_BITS_6 = 1U,
	UART_CFG_DATA_BITS_7 = 2U,
	UART_CFG_DATA_BITS_8 = 3U,
	UART_CFG_DATA_BITS_9 = 4U,
};

/**
 * @brief uart pl011 stop bits config enum options.
 *
 */
enum uart_pl011_config_stop_bits {
	UART_CFG_STOP_1_BIT = 0U,
	UART_CFG_STOP_2_BIT = 1U,
};

/**
 * @brief uart pl011 configuration structure.
 *
 */
struct uart_pl011_cfg {
	uint8_t parity;
	uint8_t stop_bits;
	uint8_t data_bits;
	uint8_t flow_ctrl;
};

/**
 * @brief uart pl011 interrupt callback.
 *
 * @param[out] user callback data.
 *
 */
typedef void (*uart_pl011_irq_cb_t)(void *user_data);

/**
 * @brief uart pl011 driver structure.
 *
 */
struct uart_pl011 {
	uintptr_t reg;              /*!< address to pl011 registers */
	uart_pl011_irq_cb_t irq_cb; /*!< interrupt callback function */
	void *irq_cb_data;          /*!< user data to pass to interrupt callback */
};

/**
 * @brief uart_pl011 struct initializer.
 *
 * @param _reg  register base address (uintptr_t)
 */
#define UART_PL011_INIT(_reg) {.reg = _reg}

/**
 * @brief convenient macro for declaration
 *
 * @param _name name of struct var
 * @param _reg  register base address (uintptr_t)
 */
#define UART_PL011_DECLARE(_name, _reg) struct uart_pl011 _name = UART_PL011_INIT(_reg)

/**
 * @brief Default UART configuration.
 *
 */
#define UART_PL011_DEFAULT_CFG                                                                     \
	{                                                                                          \
		.parity = 0,                                                                       \
		.stop_bits = UART_CFG_STOP_1_BIT,                                                  \
		.data_bits = UART_CFG_DATA_BITS_8,                                                 \
		.flow_ctrl = 0,                                                                    \
	}

/**
 * @brief Initialise the uart configuration.
 *
 * @param[in] uart pointer to uart instance.
 * @param[in] cfg  pointer to uart initialisation config.
 *
 * @retval 0 at success.
 * @retval -EINVAL invalid args.
 * @retval -eret other errors.
 */
int uart_pl011_init(struct uart_pl011 *uart, struct uart_pl011_cfg *cfg);

/**
 * @brief Enable the PL011 uart peripheral block.
 *
 * @param[in] uart pointer to uart instance.
 *
 * @retval 0 at success.
 * @retval -EINVAL invalid args.
 * @retval -eret other errors.
 */
int uart_pl011_enable(struct uart_pl011 *uart);

/**
 * @brief Poll until an available character is received.
 *
 * @note this is a blocking io call.
 *
 * @param[in] uart pointer to uart instance.
 *
 * @ret character received in fifo.
 */
char uart_pl011_poll_in(struct uart_pl011 *uart);

/**
 * @brief Poll until the character is transmitted.
 *
 * @note this is a blocking io call.
 *
 * @param[in] uart pointer to uart instance.
 * @param     c character to transmit.
 */
void uart_pl011_poll_out(struct uart_pl011 *uart, uint8_t c);

#ifdef __cplusplus
}
#endif

#endif // UART_PL011__H
