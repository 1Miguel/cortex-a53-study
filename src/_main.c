
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

void print_uart0(const char *s)
{
	while (*s != '\0') {                   /* Loop until end of string */
		*UART0DR = (unsigned int)(*s); /* Transmit char */
		s++;                           /* Next char */
	}
	while (1)
		;
}

void _main()
{
	*UART0LCR_H |= (UART_CLR_H_WLEN_8_BITS << UART_CLR_H_WLEN_BIT_POS) | UART_CLR_H_FEN_BIT_MSK;
	*UART0CR |= (UART_CR_UARTEN_BIT_MSK | UART_CR_TXE_BIT_MSK | UART_CR_RXE_BIT_MSK);
	print_uart0("Hello world!\n");
}
