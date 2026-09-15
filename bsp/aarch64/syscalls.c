/**
 * @file syscalls.c
 * @brief syscall stubs.
 *
 * Why do we need this? The C standard library (libc) must be portable
 * accross platform. But the C standard library depends on underlying
 * os (operating system) subroutines. This underlying subroutines are
 * os/platform specific. Usually if you are coding C in linux/windows
 * this subroutines are already implemented and you dont have to think
 * about it.
 *
 * But on cases wherein these subroutines are not implemented i.e using
 * standard libc in baremetal (no os), you must atleast provide do-nothing
 * implementation of the subroutines to allow your program to link with
 * libc. On such cases, this do-nothing subroutines must atleast return
 * error codes to ensure graceful failure.
 *
 * Refer to The Red Hat newlib C Library - libc documentation.
 * NOTE: It is very important to read the documentation
 * especially the Chapter 12: System Calls
 */
#include <sys/stat.h>

/**
 * @brief Write to a file.
 *
 * ‘libc’ subroutines will use this system routine for output to all
 * files, including stdout—so if you need to generate any output, for
 * example to a serial port for debugging, you should make your minimal
 * write capable of doing this.
 *
 * The following minimal implementation is an incomplete example;
 * it relies on a outbyte subroutine (not shown; typically, you must
 * write this in assembler from examples provided by your hardware
 * manufacturer) to actually perform the output.
 *
 * @return length of character written to file.
 */
int __attribute__((weak)) _write(int file, char *ptr, int len)
{
	char out;
	for (int i = 0; i < len; i++) {
		out = *(ptr++);
	}
	return len;
}

/**
 * @brief close a file
 */
int __attribute__((weak)) _close(int file)
{
	return -1;
}

/**
 * @brief Status of an open file.
 */
int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 * @brief Query whether output stream is a terminal.
 */
int _isatty(int file)
{
	return 1;
}

/**
 * @brief Set position in a file.
 */
int _lseek(int file, int ptr, int dir)
{
	return 0;
}

/**
 * @brief Read from a file.
 */
int __attribute__((weak)) _read(int file, char *ptr, int len)
{
	return 0;
}

/**
 * @brief Exit a program without cleaning up files.
 * If your system doesn’t provide this, it is best to avoid linking
 * with subroutines that require it (exit, system).
 */
void __attribute__((weak)) _exit(int status)
{
	while (1)
		;
}

/**
 * @brief Process-ID; this is sometimes used to generate strings
 * unlikely to conflict with other processes.
 */
int _getpid(void)
{
	return 1;
}

/**
 * @brief Send a kill signal.
 */
int _kill(int pid, int sig)
{
	return -1;
}

/**
 * @brief Increase program data space.
 *
 * As malloc and related functions depend on this, it is useful
 * to have a working implementation.
 *
 * @note The following suffices for a standalone system; it
 * exploits the symbol `__heap_start` automatically defined
 * by the linker.
 *
 */
void *_sbrk(int incr)
{
	extern char __heap_start; // Defined in your linker script
	extern char stack_top;
	static char *stack_ptr = &stack_top;
	static char *heap_end = &__heap_start;
	char *prev_heap_end;

	prev_heap_end = heap_end;
	if ((heap_end + incr) > stack_ptr) {
		_write(1, "Heap and stack collision\n", 25);
		while (1)
			;
		_exit(-1);
	}

	heap_end += incr;

	return (void *)prev_heap_end;
}
