#include <stdint.h>
#include <stdio.h>
#include "opena64/gic.h"
#include "opena64/arm64.h"
#include "opena64/timer.h"
#include "opena64/uart_pl011.h"
#include "opena64/assert.h"

/**
 * Refer to PrimeCell UART (PL011) r1p5 Technical Reference Manual
 */
#define UART0 (0x09000000)

struct uart_pl011 uart_dev = UART_PL011_INIT(UART0);

char *data = "hello world!";

int _write(int file, char *ptr, int len)
{
	(void)(file); // unused
	int i;
	for (i = 0; i < len; i++) {
		uart_pl011_poll_out(&uart_dev, (*ptr++));
	}
	return i;
}

void uart_cb(struct uart_pl011 *uart, void *cb_data)
{
  int n;
  uint8_t *rx_buf = cb_data;
  uint8_t *b;

  while (uart_pl011_rx_fifo_ready(uart)) {

    n = uart_pl011_rx_fifo(uart, rx_buf, 32);
    __ASSERT(!n || (n < 0));

    b = rx_buf;
    while (n--) 
      uart_pl011_poll_out(uart, *b++);
  }
}

void timer_cb(void *cb_data)
{
	printf("timer irq!\n");
	uint64_t *period_cnt = cb_data;
	//timer_set_relative((uint32_t)*period_cnt);
}

int main(void)
{
	// timer interrupts
	gic_v3_int_set_priority(30, 0);
	gic_v3_int_enable(30);

	// uart interrupts
	gic_v3_int_set_priority(33, 0);
	gic_v3_int_enable(33);

  uint8_t rx_buf[32];
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
  uart_pl011_set_irq_cb(&uart_dev, uart_cb, rx_buf);
	uart_pl011_enable(&uart_dev);
  uart_pl011_rx_irq_enable(&uart_dev);

	uint64_t period_cnt = timer_ms_to_cnt(1000);
	timer_set_irq_cb(timer_cb, &period_cnt);
	timer_start();

	printf("hi! this is cpu: %d at %d lvl\n", curr_cpu_id(), curr_exc_lvl());
	printf("here's a message: %s\n", data);
	printf("end\n");

	timer_set_relative((uint32_t)period_cnt);

	while (1) {
    //uart_pl011_poll_out(&uart_dev, uart_pl011_poll_in(&uart_dev));
	}

	return 0;
}

void int_33_irq_handler(void)
{
  uart_pl011_irq_handler(&uart_dev);
}
