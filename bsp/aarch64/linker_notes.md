# Memory Layout Structure

## CPU's view of memory
CPU's view of memory is one big linear array of bytes, nothing more, nothing less. But usually, CPU manufacturer/designer would provide a guide of the memory model of a particular CPU architecture. 

A memory model is a way of organizing and defining how memory behaves. It provides a structure and a set of rules for you to follow when you configure how addresses, or regions of addresses, are accessed and used in your system. 

Below is an example:

```text
  +---------------+
  |               | Read/Write (non-executable)
  |  Peripherals  | Priviledge Access only
  |               | None-Cachable
  +---------------+
  |               |
  |     ...       |   
  |               |
  +---------------+
  |               | Read/Write (non-executable)
  |  Data (RAM)   | Unpriviledge
  |               | Cachable
  +---------------+
  |               | Read-only (executable)
  |  Code (ROM)   | Unpriviledge
  |               | Cachable
  +---------------+
```

The above image is called an address map. An address map is the arrangement of memory regions within an address space. Usually Code(ROM) region will start at 0x00000000 then followed by Data and Peripheral space. It is up to the implementer how to arrange the address space and the addresses to assign to each region.

> [!NOTE] Reference
> For ARMv8 refer to Learn the architecture - AArch64 memory model

## Programs view of memory
> [!NOTE] We are discussing this in BareMetal PoV.

A program that CPU execute has a logical view of memory. It views the memory in **sections**.

```text
  +---------+ ---+
  | .stack  |    |
  +---------+    |
  | .heap   |    |
  +---------+    +- RAM
  | .bss    |    |
  +---------+    |
  | .data   |    |
  +---------+ ---+
  | .debug  |    |
  +---------+    |
  | .rodata |    +- ROM
  +---------+    |
  | .text   |    |
  +---------+ ---+
```

* **.text**: Contains the executable compiled code. Read-only.
* **.rodata**: Read-only data such as **strings**, **const** vars, jump tables etc.
* **.data**: **Initialized** global C vars (int var = 0;)
* **.bss**: **Uninitialized** global C vars i.e (int var;)
* **.debug**: A **debugging symbol table** with entries for local variables and typedefs defined in the program, global variables defined and referenced in the program, and the original C source file. It is only present if the compiler driver is invoked with the **-g** option.
* **.line**: A *mapping between line numbers* in the original C source program and machine code instructions in the .text section. It is only present if the compiler driver is invoked with the **-g** option.

> [!NOTE] Reference
> Computer Systems: A Programmer's Perspective 2nd ed.

# What is .init_array section?

If you try to include libc, you'll find the build will failed not until you include this in the linker file ... but why?
```ld
.init_array : ALIGN(8)
{
    __init_array_start = .;
    KEEP(*(.init_array*))
    __init_array_end = .;
} > FLASH
```

If you look into **libc** source code (newlib/libc/misc/init.c), you will the function below:

``` C
void __libc_init_array (void)
{
    size_t count;
    size_t i;

    count = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < count; i++)
        __preinit_array_start[i] ();

    _init ();

    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++)
        __init_array_start[i] ();
}
```

This function iterates and execute all initialization routines. If g++ is used, this will contain all constructor routines for statically defined class objects.

In our case, we can inspect the **program.elf** via **objdump**:
```bash
> aarch64-none-elf-objdump -D -j .init_array ./build/firmware.elf

./build/firmware.elf:     file format elf64-littleaarch64


Disassembly of section .init_array:

000000000000db58 <__frame_dummy_init_array_entry>:
    db58:       000006a0        udf     #1696
    db5c:       00000000        udf     #0
    db60:       00002280        udf     #8832
    db64:       00000000        udf     #0

```
>[!NOTE]Options Meaning
>   (-D | --disassemble-all)
>     Disassemble the contents of all none empty non-bss sections.
>   (-j <section> | --section=<section>)
>     Display information for given section name.

It provides a list of addresses that is within the target section. To get detailed information to these addresses, we can use **addr2line**.

``` bash
aarch64-none-elf-addr2line 0xdb58 -e ./build/firmware.elf
aarch64-none-elf-addr2line 0xdb5c -e ./build/firmware.elf
aarch64-none-elf-addr2line 0xdb60 -e ./build/firmware.elf
aarch64-none-elf-addr2line 0xdb64 -e ./build/firmware.elf

```

>[!NOTE]Options Meaning
> (-e <filename> | -exe=<filename>)
>   input executable program.
