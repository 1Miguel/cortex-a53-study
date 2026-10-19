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

struct uart_pl011;

enum uart_pl011_err_msk {
	UART_PL011_INT_STATUS_FR_ERR = 1U << 7,
	UART_PL011_INT_STATUS_PARITY_ERR = 1U << 8,
	UART_PL011_INT_STATUS_BRK_ERR = 1U << 9,
	UART_PL011_INT_STATUS_OVR_ERR = 1U << 10,
};

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
	uint32_t baudrate;
	uint8_t parity;
	uint8_t stop_bits;
	uint8_t data_bits;
	uint8_t flow_ctrl;
};

/**
 * @brief uart pl011 interrupt callback.
 *
 * @param[in]  pointer to uart instance.
 * @param[in]  user callback data.
 * @param[out] interrupt reason.
 *
 */
typedef void (*uart_pl011_irq_cb_t)(struct uart_pl011 *, void *);

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
		.baudrate = 115200,                                                                \
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
 * @brief Initialise the uart configuration.
 *
 * @param[in] uart pointer to uart instance.
 * @param[in] cb
 * @param[in] cb_data
 *
 * @retval 0 at success.
 * @retval -EINVAL invalid args.
 * @retval -eret other errors.
 */
int uart_pl011_set_irq_cb(struct uart_pl011 *uart, uart_pl011_irq_cb_t cb, void *cb_data);

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
int uart_pl011_poll_out(struct uart_pl011 *uart, uint8_t c);

/**
 * @brief Enable interrupts based on given mask.
 *
 * @param[in] uart pointer to uart instance.
 * @param     irq_msk interrupt mask to enable.
 *
 * @retval 0 at success
 * @retval -EINVAL invalid args.
 */
int uart_pl011_tx_irq_enable(struct uart_pl011 *uart);
int uart_pl011_tx_irq_disable(struct uart_pl011 *uart);

int uart_pl011_rx_irq_enable(struct uart_pl011 *uart);
int uart_pl011_rx_irq_disable(struct uart_pl011 *uart);

int uart_pl011_err_irq_enable(struct uart_pl011 *uart);
int uart_pl011_err_irq_disable(struct uart_pl011 *uart);

/**
 * @brief Check and return error code.
 *
 * @retval 0 no error.
 * @retval err error flags defined in @ref uart_pl011_err_msk,
 * @retval -EINVAL invalid args.
 */
int uart_pl011_err_chk(struct uart_pl011 *uart);

/**
 * @brief Fill transmit fifo with data.
 *
 * @param[in] uart pointer to uart instance.
 * @param[in] buf pointer to buffer to be written to fifo.
 * @param     len number of bytes to be wrtten to fifo.
 *
 * @retval n number of bytes written in fifo.
 * @retval -EINVAL invalid args
 */
int uart_pl011_tx_fifo(struct uart_pl011 *uart, uint8_t *buf, uint16_t len);

/**
 * @brief Check if tx fifo is not empty and ready to be filled.
 *     data will immediately be transmitted over uart tx once filled.
 *
 * @param[in] uart pointer to uart instance.
 *
 * @retval 0 if tx fifo is not ready
 * @retval 1 if tx fifo is ready
 * @retval -EINVAL if args invalid
 * @retval -err other errors
 */
int uart_pl011_tx_fifo_ready(struct uart_pl011 *uart);

/**
 * @brief Read from rx fifo.
 *
 * @param[in] uart pointer to uart instance.
 *
 * @retval n number of bytes read from rx fifo.
 * @retval -EINVAL if args invalid
 * @retval -err other errors
 */
int uart_pl011_rx_fifo(struct uart_pl011 *uart, uint8_t *buf, uint16_t len);

/**
 * @brief Check if rx fifo has data and ready to be read.
 *
 * @param[in] uart pointer to uart instance.
 *
 * @retval 0 if rx fifo is not ready
 * @retval 1 if rx fifo is ready
 * @retval -EINVAL if args invalid
 * @retval -err other errors
 */
int uart_pl011_rx_fifo_ready(struct uart_pl011 *uart);

/**
 * @brief UART interrupt handler. This must be called in the
 *    PL011 interrupt service routine.
 *
 * @param[in] uart pointer to uart instance.
 */
void uart_pl011_irq_handler(struct uart_pl011 *uart);

#ifdef __cplusplus
}
#endif

#endif // UART_PL011__H
