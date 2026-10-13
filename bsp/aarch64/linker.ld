/**
 * @file liker.ld
 * @brief This is the linker file that describes the memory
 * layout of cortex-a53. This is meant to work with renode,
 *
 * Refer to:
 * https://github.com/renode/renode/blob/master/platforms/cpus/cortex-a53-gicv3.repl
 */

/* Tells the linker that this is the program entry point */
ENTRY(_reset)


MEMORY {
  FLASH (rx)   : ORIGIN = 0x00000000, LENGTH = 0x08000000
  GICD  (rw)   : ORIGIN = 0x08000000, LENGTH = 0x00010000
  GICR  (rw)   : ORIGIN = 0x080A0000, LENGTH = 0x00010000
  RAM   (rw)   : ORIGIN = 0x40000000, LENGTH = 0x80000000
  BL31  (rwx)  : ORIGIN = 0x0e000000, LENGTH = 0x01000000
}

PHDRS {
  flash PT_LOAD FLAGS(5);
  ram   PT_LOAD FLAGS(6);
}

SECTIONS
{
  PROVIDE(__gicd_base = ORIGIN(GICD));
  PROVIDE(__gicr_base = ORIGIN(GICR));

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
  } > FLASH :flash

  . = ALIGN(8);
  .rodata :{
    .__rodata_start__ = .;
    *(.rodata*)
    .__rodata_end__ = .;
  } > FLASH :flash

  /*
   * C/C++ initialization arrays.
   */
  .preinit_array : {
    . = ALIGN(64);
    __preinit_array_start = .;
    KEEP (*(SORT(.preinit_array.*)))
    KEEP (*(.preinit_array))
    __preinit_array_end = .;
  } > FLASH :flash

  /**
   * What is this for?
   *
   * if you look into libc source code, you will find
   * a code snippet
   */
  .init_array : {
    . = ALIGN(64);
    __init_array_start = .;
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    __init_array_end = .;
  } > FLASH :flash

  .fini_array : {
    . = ALIGN(64);
    __fini_array_start = .;
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array))
    __fini_array_end = .;
  } > FLASH :flash

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
  } > RAM AT > FLASH :ram

  . = ALIGN(8);
	.bss (NOLOAD) : {
    __bss_start__ = .;
    *(.bss*)
    *(.bss COMMON)
    __bss_end__ = .;
  } > RAM :NONE

  . = ALIGN(__bss_end__, 8);
  .heap (NOLOAD) : {
	  __heap_start = .;
    . += 0x1000;
    . = ALIGN(8);
	  __heap_end = .;
  } > RAM :NONE

  . = ALIGN(16);
  __stack_bottom_el3 = .;
  . += 32K;
  __stack_top_el3 = .;

  . = ALIGN(16);
  __stack_bottom_el2 = .;
  . += 32K;
  __stack_top_el2 = .;

  . = ALIGN(16);
  __stack_bottom_el1 = .;
  . += 32K;
  __stack_top_el1 = .;

  /**
  * start of the data section loadable memory
  */
  __data_load_start__ = LOADADDR(.data);
}
