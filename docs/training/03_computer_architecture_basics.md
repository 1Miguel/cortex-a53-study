# Computer Architecture Basics

## Module overview

Computer architecture describes how software-visible components of a computer
work together to execute instructions. This module introduces the processor,
memory, buses, interrupts, and boot flow, using the Cortex-A53 study platform
as a running example.

### Learning objectives

By the end of this module, you should be able to:

- Describe the main components of a computer system and how they communicate.
- Explain the instruction fetch, decode, and execute cycle at a high level.
- Distinguish registers, caches, RAM, ROM/flash, and memory-mapped devices.
- Explain the role of addresses, load/store instructions, and the memory map.
- Describe what an interrupt does and how it differs from a function call.
- Outline the steps from reset to a C `main()` function.

### Prerequisites

Basic programming knowledge is helpful. Familiarity with C is useful for the
examples, but the concepts are not language-specific.

## 1. A computer system's building blocks

A simple computer system contains:

- **Processor (CPU):** fetches and executes instructions.
- **Registers:** small, fast storage locations inside the processor.
- **Memory:** stores instructions and data.
- **Interconnect:** carries addresses, data, and control information between
  components.
- **Peripherals:** devices such as UARTs, timers, storage controllers, and
  interrupt controllers.

The processor executes instructions, but most programs also need to read and
write memory and communicate with peripherals. The system's hardware and
firmware define which addresses correspond to which resources.

## 2. Instructions and the processor

At a high level, the processor repeatedly:

1. Fetches an instruction from memory using the program counter.
2. Decodes the instruction to determine the requested operation.
3. Executes it, possibly reading or writing registers or memory.
4. Advances or changes the program counter.

The program counter is the address of the instruction stream. Branch
instructions change where execution continues. A function call is typically
implemented as a branch that also saves a return address, allowing execution to
continue after the function returns.

Modern processors use pipelines, caches, and other techniques to improve
performance, so the underlying hardware may perform multiple operations at
once. The architectural behavior is the programmer-visible contract; it does
not require that each instruction execute as one indivisible internal step.

## 3. Registers, memory, and storage

Different storage types have different roles:

| Resource | Typical role |
| --- | --- |
| General-purpose registers | Operands and temporary values used by instructions |
| CPU cache | Copies of recently used memory contents to reduce access latency |
| RAM | Writable run-time storage for code and data |
| ROM or flash | Persistent storage often used for firmware and constants |
| Peripheral registers | Control and status for hardware devices |

The exact properties—capacity, latency, persistence, and access rules—depend on
the system. C variables do not each correspond to a dedicated hardware
register: the compiler chooses whether to keep a value in a CPU register or in
memory.

## 4. Address spaces and the memory map

An address is a number used to identify a location in an address space. A
platform's memory map assigns ranges of addresses to resources. For example,
the study platform maps:

- Flash at `0x00000000`.
- RAM at `0x40000000`.
- PL011 UART at `0x09000000`.
- GIC distributor at `0x08000000`.
- GIC redistributor region beginning at `0x080A0000`.

These are platform addresses, not universal Cortex-A53 addresses. A different
SoC or virtual machine may map its devices elsewhere.

On AArch64, ordinary data loads and stores access memory. The same instruction
forms can access a peripheral when its address is mapped as device memory and
the platform permits the access. Software must use the register width,
alignment, and access semantics documented for that device.

## 5. Caches and memory ordering

Caches keep copies of memory to speed up access. They are usually transparent
for ordinary program behavior, but firmware must consider cache state when
loading code, sharing data with devices, or changing memory attributes.

Processors can also reorder or combine some memory operations when the
architecture allows it. A program that communicates with a device or another
processor may need memory barriers, cache maintenance, or synchronization
primitives. `volatile` in C prevents certain compiler optimizations of an
access; it does not perform cache maintenance or provide all required hardware
ordering.

Memory regions are assigned attributes such as normal cacheable memory or
device memory. Correct attributes are important: treating a peripheral as
ordinary cacheable RAM can lead to unexpected behavior.

## 6. Exceptions and interrupts

An **exception** is a change in normal instruction flow caused by an event.
Examples include synchronous faults, system calls, and interrupts.

- A **synchronous exception** is associated with the instruction being
  executed, such as an invalid memory access.
- An **interrupt** is an asynchronous event, commonly raised by a timer or
  peripheral to request service.

An interrupt is not a normal function call. The processor saves exception
state, transfers execution to an exception vector, and software determines
which source needs service. The handler acknowledges or services the source
and returns using the architecture's exception-return mechanism.

On Armv8-A, exceptions are handled at an Exception Level (EL). EL0 is commonly
used for applications, EL1 for an operating system or firmware, EL2 for
hypervisor software, and EL3 for secure monitor firmware. Which ELs are
implemented and how security states are configured depend on the processor and
platform. An EL is a privilege level, not a synonym for Secure or Non-secure
state.

The Generic Interrupt Controller (GIC) collects and prioritizes interrupts.
The device raises an interrupt, the GIC identifies and routes it, and the CPU
takes an exception if the required enables and masks allow delivery.

## 7. Boot flow: reset to C

A bare-metal C program needs a small amount of startup work before `main()`:

1. The platform starts execution at a reset entry address.
2. Startup assembly establishes a stack and any required processor state.
3. Initialized data is copied to its run-time location in RAM.
4. The `.bss` region is cleared to zero.
5. Runtime initialization functions are called when required.
6. Startup code branches to `main()`.

The linker script assigns addresses to sections such as `.text`, `.rodata`,
`.data`, and `.bss`, and describes how their contents are loaded. The startup
code must agree with that layout. For example, it must copy `.data` from the
linker-defined load address to its run-time address.

Unlike a hosted application, a bare-metal firmware image cannot assume that an
operating system has initialized its C runtime or provided services such as
console output. Firmware supplies platform-specific implementations for the
services it needs.

## 8. Exercises

1. Draw a block diagram showing the CPU, RAM, flash, UART, and GIC. Label which
   components execute instructions and which provide data or events.
2. Use the platform's Renode or QEMU description to find the address ranges
   for RAM, UART, and GIC. Explain why those addresses are platform-specific.
3. Trace a function call in GDB: inspect the program counter before the call,
   inside the function, and after it returns.
4. Put a breakpoint on an exception handler and distinguish the saved return
   address and fault address registers for a synchronous abort.
5. Locate the reset entry, data-copy loop, BSS-clear loop, and branch to `main`
   in the startup assembly.
6. Explain why a timer interrupt can occur between two ordinary C statements
   and why its handler must preserve the processor's exception state.

## Key takeaways

- The processor executes instructions and uses registers and memory to hold
  program state.
- A platform memory map assigns addresses to RAM, firmware storage, and
  peripherals.
- Caches and memory attributes affect how memory accesses behave and perform.
- Exceptions and interrupts redirect execution through architecture-defined
  mechanisms.
- Bare-metal startup, linker layout, and platform firmware together establish
  the environment in which C code runs.

## Further reading

- [Armv8-A AArch64 memory model notes](../arm_notes/arm_memory.md)
- [AArch64 boot flow](../arm_notes/aarch64-bootflow.md)
- [Exception handling notes](../arm_notes/exception_handling.md)
- [Cortex-A53 study platform README](../../README.md)
