#include <stdint.h>
#include <stdio.h>
#include "gic.h"
#include "opena64/arm64.h"
#include "opena64/arm_gen_timer.h"
#include "opena64/uart_pl011.h"

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

int main(void)
{
	// gic_v3_init();

	uint64_t period_cnt = arm_gen_timer_ms_to_cnt(1000);
	struct uart_pl011_cfg uart_cfg = UART_PL011_DEFAULT_CFG;
	uart_pl011_init(&uart_dev, &uart_cfg);
	uart_pl011_enable(&uart_dev);

	arm_gen_timer_start();

	printf("hi! this is cpu: %d at %d lvl\n", curr_cpu_id(), curr_exc_lvl());
	printf("here's a message: %s\n", data);
	printf("end\n");

	while (1) {
		arm_gen_timer_set_relative((uint32_t)period_cnt);
		arm_gen_timer_poll();
		printf("timer timeout!\n");
	}

	return 0;
}
