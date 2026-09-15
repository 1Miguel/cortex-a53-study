/**
 * @file liker.ld
 * @brief This is the linker file that describes the memory
 * layout of cortex-a53. This is meant to work with renode,
 *
 * Refer to:
 * https://github.com/renode/renode/blob/master/platforms/cpus/cortex-a53-gicv3.repl
 */

/* Tells the linker that this is the program entry point */
ENTRY(_Reset)

MEMORY {
  FLASH (rx)   : ORIGIN = 0x00000000, LENGTH = 0x08000000
  RAM   (rw!x) : ORIGIN = 0x40000000, LENGTH = 0x80000000
  BL31  (rwx)  : ORIGIN = 0x0e000000, LENGTH = 0x01000000
}

SECTIONS
{
  . = ALIGN(ORIGIN(FLASH), 8);

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

  /**
   * Notice the `AT` operator? It means that at runtime
   * the .data segment is exepcted to be found at RAM, but 
   * at load time it's at FLASH. We call its address at runtime
   * the VMA (Virtual Memory Address) and its address at FLASH
   * as the LMA (Loadable Memory Address).
   * 
   * It is the responsibility of the startup code or initialization
   * code that the `.data` segment in RAM is initialised with
   * values stored in FLASH before code starts.
   *
   * ... but why? its because all C static variables are stored in
   * `.data` segment.
   */
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
	__heap_start = . + 0x8000;

  . = ALIGN(8);
	stack_top = ORIGIN(RAM) + LENGTH(RAM);

  /**
  * start of the data section loadable memory
  */
  __data_load_start__ = LOADADDR(.data);
}
