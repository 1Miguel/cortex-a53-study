# Low-Level C Programming for Bare-Metal AArch64

## Module overview

This module introduces the C features used to write firmware that interacts
directly with memory, peripherals, and processor startup code. Examples target
the Cortex-A53 study platform, but the C concepts apply to other embedded
systems.

### Learning objectives

By the end of this module, you should be able to:

- Explain how C objects map to addresses and bytes in memory.
- Use fixed-width integer types and bit operations for hardware registers.
- Access memory-mapped registers with appropriately qualified pointers.
- Distinguish `volatile` from atomicity, synchronization, and memory barriers.
- Recognize why initialization, alignment, and linker layout matter to C code.
- Inspect compiled code and debug memory values.

### Prerequisites

Basic C syntax, integer arithmetic, functions, and familiarity with the
processor's address map are assumed. See [Memory Layout Structure](../arm_notes/arm_memory.md)
for background on memory sections.

## 1. C objects, addresses, and pointers

A C object occupies storage and has a type. The type determines how C interprets
the object's value and, for pointer arithmetic, how far to advance.

```c
#include <stdint.h>

uint32_t counter = 7;
uint32_t *counter_ptr = &counter;

*counter_ptr = *counter_ptr + 1;
```

`&counter` is the address of the object. `*counter_ptr` accesses the object at
that address. Since `counter_ptr` points to `uint32_t`, adding one to it advances
by `sizeof(uint32_t)` bytes, not one byte.

An integer containing an address is not automatically a valid pointer. Only
convert an address to a pointer when the platform or interface defines that
address as accessible and the pointer type and access width are appropriate.

## 2. Use types that express hardware widths

Use `<stdint.h>` types such as `uint8_t`, `uint32_t`, and `uint64_t` when the
width matters. Avoid assuming that plain `int`, `long`, or a pointer has the
same width on every target.

```c
uint32_t status;
uint64_t counter_value;
```

Use `uintptr_t` for integer representations of pointers when supported:

```c
#include <stdint.h>

#define UART0_BASE ((uintptr_t)0x09000000)
```

Register maps specify access widths and reserved bits. Follow the peripheral's
documentation; do not assume every register can safely be read or written as an
arbitrary C type.

## 3. Bit masks and register fields

Bit masks make individual fields easier to set, clear, and test. Use unsigned
constants and shift by a value less than the width of the left operand.

```c
#define UART_ENABLE   (1u << 0)
#define UART_TX_ENABLE (1u << 9)

uint32_t control = 0;
control |= UART_ENABLE;             /* Set a bit. */
control &= ~UART_TX_ENABLE;         /* Clear a bit. */
bool enabled = (control & UART_ENABLE) != 0; /* Test a bit. */
```

For a multi-bit field, clear the old field before inserting a new value:

```c
#define FIELD_MASK  (0x3u << 5)
#define FIELD_SHIFT 5

uint32_t value = 0;
value = (value & ~FIELD_MASK) | ((field_value << FIELD_SHIFT) & FIELD_MASK);
```

The register specification determines which bits are writable and what a write
means. Some registers are write-one-to-clear, read-only, or have other
side-effects. A read-modify-write expression such as `reg |= mask` is only
appropriate if the peripheral permits reading the register and writing back
the other bits unchanged.

## 4. Memory-mapped I/O

Memory-mapped peripherals expose registers at addresses in the processor's
physical address space. On this platform, the PL011 UART data register is at
`0x09000000`.

```c
#include <stdint.h>

#define UART0_BASE ((uintptr_t)0x09000000)
#define UART0_DR   (*(volatile uint32_t *)UART0_BASE)

void uart_put_byte(uint8_t byte)
{
    UART0_DR = byte;
}
```

`volatile` tells the compiler that each access is observable and must not be
optimized away or cached as an ordinary C value. It is appropriate for
memory-mapped registers.

`volatile` does **not** guarantee that:

- An access is atomic.
- Multiple accesses happen in a particular order relative to other observers.
- CPU caches are bypassed or memory attributes are configured correctly.
- A peripheral is ready to accept the access.

Use the architecture's barriers, atomic operations, locking, or memory
attributes when those guarantees are required. For device registers, also obey
the peripheral's required access width and wait for status such as a FIFO-full
bit before writing.

## 5. Alignment, layout, and initialization

Objects have alignment requirements. A `uint32_t` normally needs an address
aligned for 32-bit access; packed structures or manually chosen addresses can
violate that requirement. Hardware register layouts should follow the device
specification, and C structures used to model them must be checked for padding
and offsets rather than assumed to match.

Bare-metal programs also need startup code to establish the C environment
before ordinary C functions run:

1. Set a valid, suitably aligned stack pointer.
2. Copy initialized data from its load address to its run-time address.
3. Clear the `.bss` region.
4. Run C initialization routines if the linked runtime uses them.
5. Call `main`.

Global initialized variables are generally placed in `.data`; zero-initialized
and uninitialized globals are generally placed in `.bss`; constants may be
placed in `.rodata`. The linker script defines where these sections live, while
startup code initializes the sections that need it. See
[the AArch64 linker notes](../../bsp/aarch64/linker_notes.md).

## 6. Compiler and generated code

C describes behavior, not a fixed sequence of machine instructions. The
compiler may keep ordinary variables in registers, combine operations, or
remove work that cannot affect observable behavior. `volatile` changes the
compiler's treatment of the specific volatile accesses; it does not disable
optimization for the whole program.

For bare-metal AArch64 builds, flags such as `-ffreestanding` describe the
environment to the compiler. `-mgeneral-regs-only` prevents compiler-generated
floating-point and Advanced SIMD register use in the C files compiled with
that flag. It does not alter prebuilt libraries or hand-written assembly.

Inspect generated code when behavior is unclear:

```sh
aarch64-none-elf-objdump -d build_aarch64/firmware.elf
```

Use GDB to inspect an address directly:

```gdb
x/4wx 0x09000000
x/gx 0x40000000
x/i $pc
```

Reading a peripheral register can itself have side effects, so only inspect
MMIO addresses when the device documentation says the read is safe.

## 7. Exercises

1. Declare a `uint32_t` variable and a pointer to it. Print or inspect both the
   pointer value and the pointed-to value in GDB.
2. Write helper expressions to set, clear, and test a selected bit in a
   `uint32_t`. Verify the result with a debugger.
3. Read the PL011 register map and identify the data, flag, and control
   registers. Explain why software should check the transmit FIFO status before
   writing a byte.
4. Find the `.data` and `.bss` boundaries in the linker script and match them
   to the startup initialization code.
5. Disassemble a small function that uses a volatile register pointer and
   identify the load/store instructions the compiler emitted.

## Key takeaways

- A pointer gives C a typed way to access an address; it does not make every
  address valid.
- Fixed-width unsigned types and explicit masks are useful for register work.
- `volatile` is for observable accesses, especially MMIO; it is not a general
  synchronization or hardware-ordering primitive.
- Correct low-level C depends on the register specification, processor
  configuration, startup code, and linker layout as well as the C source.
