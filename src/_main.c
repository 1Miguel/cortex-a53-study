#include <stdio.h>

/**
 * Refer to PrimeCell UART (PL011) r1p5 Technical Reference Manual
 */
volatile unsigned int *const UART0DR = (unsigned int *)0x09000000;
volatile unsigned int *const UART0LCR_H = (unsigned int *)(0x09000000 + 0x2C);
volatile unsigned int *const UART0CR = (unsigned int *)(0x09000000 + 0x30);

#define UART_CR_UARTEN_BIT_MSK (1U << 0)
#define UART_CR_TXE_BIT_MSK    (1U << 9)
#define UART_CR_RXE_BIT_MSK    (1U << 8)

#define UART_CLR_H_FEN_BIT_MSK  (1 << 4)
#define UART_CLR_H_WLEN_BIT_MSK (0x03)
#define UART_CLR_H_WLEN_BIT_POS (5)

#define UART_CLR_H_WLEN_8_BITS (0x03)

char *data = "hello world!";

int _write(int file, char *ptr, int len)
{
  (void)(file); // unused
  int i;
	for (i = 0; i < len; i++) {
		*UART0DR = (unsigned int)(*ptr++);
	}
  return i;
}

int main()
{
	*UART0LCR_H |= (UART_CLR_H_WLEN_8_BITS << UART_CLR_H_WLEN_BIT_POS) | UART_CLR_H_FEN_BIT_MSK;
	*UART0CR |= (UART_CR_UARTEN_BIT_MSK | UART_CR_TXE_BIT_MSK | UART_CR_RXE_BIT_MSK);
	printf("hi! here's a message: %s\n", data);
	while (1);
	return 0;
}
