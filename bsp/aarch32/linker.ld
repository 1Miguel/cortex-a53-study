/**
 * @file liker.ld
 * @brief This is the linker file that describes the memory
 * layout of cortex-a9 (aarch32). This is meant to work with renode,
 *
 * Refer to:
 * https://github.com/renode/renode/blob/master/platforms/cpus/cortex-a53-gicv3.repl
 */

ENTRY(_reset)

STACK_SIZE_IRQ = 16K;

MEMORY {
  FLASH (rx)   : ORIGIN = 0x80000000, LENGTH = 0x04000000
  RAM   (rw!x) : ORIGIN = 0x84000000, LENGTH = 0x04000000
}

SECTIONS
{
  . = ALIGN(ORIGIN(FLASH), 4);

  /**
   * The `.text` secition usually contains read-only
   * compiled machine code. note that you can use any
   * name to name this section, but it is preferred to
   * use`.text` since its a standardized name used by
   * c runtime (crt).
   */
	.text : {
    /* make sure startup section is at the start of FLASH
     * mem, see the `.section` directive in `startup.asm`
     *
     * ... whats with the KEEP()?
     * KEEP() ensures that even if compiler thinks startup section
     * is unused (dead code), it will not be ignored.*/
    KEEP(*(.startup))
    *(.startup*)
    *(.text)
    *(.text*)
  } > FLASH

  . = ALIGN(8);
  .rodata :{
    .__rodata_start__ = .;
    *(.rodata*)
    .__rodata_end__ = .;
  } > FLASH

  . = ALIGN(8);
	.data : {
    __data_start__ = .;
    *(.data)
    *(.data*)
    __data_end__ = .;
  } > RAM AT > FLASH

  . = ALIGN(8);
	.bss : {
    __bss_start__ = .;
    *(.bss*)
    *(.bss COMMON)
    __bss_end__ = .;
  } > RAM

  /*
   * C/C++ initialization arrays.
   */
  .preinit_array : ALIGN(8)
  {
      __preinit_array_start = .;
      KEEP(*(.preinit_array*))
      __preinit_array_end = .;
  } > FLASH

  /**
   * What is this for?
   *
   * if you look into libc source code, you will find
   * a code snippet
   */
  .init_array : ALIGN(8)
  {
      __init_array_start = .;
      KEEP(*(.init_array*))
      __init_array_end = .;
  } > FLASH

  .fini_array : ALIGN(8)
  {
      __fini_array_start = .;
      KEEP(*(.fini_array*))
      __fini_array_end = .;
  } > FLASH

  . = ALIGN(__bss_end__, 8);
  .heap : {
	  __heap_start = .;
    . += 0x1000;
    . = ALIGN(8);
	  __heap_end = .;
  }

  . = ALIGN(16);
  .cpu0.stack : {
    . += STACK_SIZE_IRQ;
    . = ALIGN(8);
    __cpu0_stack_top = .;
  } > RAM

  /**
  * start of the data section loadable memory
  */
  __data_load_start__ = LOADADDR(.data);
}
