# AArch64 Assembly Terminology Quick Lookup

This is a compact reference for reading GNU assembler (`as`) AArch64
source files, especially bare-metal startup code.

## Directives, sections, and symbols

| Term | Meaning | Example |
|---|---|---|
| `.global` / `.globl` | Makes a symbol visible to the linker and other object files. | `.global _start` |
| `.type` | Describes a symbol's kind to the assembler and linker. | `.type _start, %function` |
| `.size` | Records a symbol's size. | `.size _start, . - _start` |
| `.section` | Selects or creates a section in the object file. | `.section .text` |
| `.text` | Code section, usually executable and read-only. | `.text` |
| `.rodata` | Read-only data, such as strings and constant tables. | `.section .rodata` |
| `.data` | Initialized writable data. | `.section .data` |
| `.bss` | Zero-initialized or uninitialized storage. | `.section .bss` |
| `.align` | Aligns the next item; its argument can be target/toolchain-dependent. | `.align 3` |
| `.balign` | Aligns the next item to a byte boundary. | `.balign 16` |
| `.space` | Reserves a number of bytes, optionally filling them. | `.space 4096` |
| `.word` | Emits a 32-bit value. | `.word 0x12345678` |
| `.quad` | Emits a 64-bit value. | `.quad 0x1234` |
| `.ascii` / `.asciz` | Emits text; `.asciz` also appends a NUL byte. | `.asciz "hello"` |
| `.equ` / `.set` | Defines a symbolic constant. | `.equ UART_BASE, 0x09000000` |
| `.end` | Marks the end of the source file for the assembler. | `.end` |

### Alignment

`.balign N` advances the location counter until it is a multiple of `N`.
For example, `.balign 16` places the next symbol at a 16-byte boundary.
Padding bytes are normally zero unless a fill value is supplied.

## Labels and common linker symbols

| Term | Meaning |
|---|---|
| `label:` | A named address at the current location. Instructions can branch to it. |
| `_start` | A conventional entry-point name. The linker script usually selects it as the first code to run. |
| `stack_top` | Usually a label marking the first address above reserved stack memory. It is a project-defined symbol, not an AArch64 instruction or register. |
| `__bss_start__` / `__bss_end__` | Common linker-defined symbols delimiting the BSS section. Exact names depend on the linker script. |
| `.` | The assembler's current location counter. In `. - label`, it means the current address minus `label`. |
| `@function` / `@object` | ELF symbol type annotations used with directives such as `.type`. |

Example stack reservation:

```asm
.section .bss
.balign 16
stack_bottom:
    .space 4096
stack_top:

.section .text
.global _start
_start:
    mov sp, stack_top
```

The stack grows toward lower addresses on AArch64, so `sp` is initialized to
the high end, `stack_top`, of the reserved region.

## Registers

| Term | Meaning |
|---|---|
| `x0`-`x30` | 31 general-purpose 64-bit registers. |
| `w0`-`w30` | The low 32 bits of `x0`-`x30`. Writing a `w` register clears the upper 32 bits of its `x` register. |
| `xzr` | The 64-bit zero register. Reads as zero; writes are discarded. |
| `wzr` | The 32-bit zero register. Reads as zero; writes are discarded. |
| `sp` | Stack pointer. It is not interchangeable with every general-purpose register in every instruction. |
| `x30` / `lr` | Link register. `bl` places the return address in it. |
| `pc` | Program counter. AArch64 does not provide a general-purpose `pc` register for ordinary `mov` instructions. |
| `x29` / `fp` | Frame pointer by common ABI convention; not enforced by hardware. |
| `nzcv` | Condition flags: Negative, Zero, Carry, and oVerflow. |
| `elr_el1` | Exception Link Register for Exception Level 1; stores the return address from an exception. |
| `spsr_el1` | Saved Program Status Register for Exception Level 1. |

Examples:

```asm
mov x0, xzr       // x0 = 0
mov w0, wzr       // lower 32 bits of x0 = 0
mov x1, sp        // copy the stack pointer
ret               // return to the address in x30/lr
```

## Operand and syntax notation

| Term | Meaning | Example |
|---|---|---|
| `#imm` | Immediate constant encoded in the instruction. | `add x0, x0, #1` |
| `Rn`, `Rm`, `Rd` | Conventional names for source, second source, and destination registers. | `add Rd, Rn, Rm` |
| `[xN]` | Memory addressed by the value in `xN`. | `ldr x0, [x1]` |
| `[xN, #offset]` | Memory at base register plus an offset. | `ldr x0, [x1, #8]` |
| `!` | Write back the updated address to the base register. | `ldr x0, [x1, #8]!` |
| `label` | A symbolic address resolved by the assembler/linker. | `b loop` |
| `=` | GNU assembler literal-load pseudo-syntax. | `ldr x0, =UART_BASE` |
| `:` | Separates a label name from its address. | `loop:` |
| `//` | Comment in AArch64 source. | `// initialize stack` |

## Common instructions

| Instruction | Meaning |
|---|---|
| `mov` | Move a register or immediate value. Often an assembler alias for another instruction. |
| `adr` | Compute a nearby PC-relative address. |
| `adrp` | Compute a page-aligned PC-relative address; commonly paired with `add`. |
| `ldr` / `str` | Load from / store to memory. `ldr x0` accesses 64 bits; `ldr w0` accesses 32 bits. |
| `ldrb` / `strb` | Load / store one byte. |
| `ldrh` / `strh` | Load / store one 16-bit halfword. |
| `ldp` / `stp` | Load / store a pair of registers, often for function prologues and epilogues. |
| `add` / `sub` | Integer addition / subtraction. |
| `and` / `orr` / `eor` | Bitwise AND / OR / exclusive OR. |
| `lsl` / `lsr` / `asr` | Logical left / logical right / arithmetic right shift. |
| `cmp` | Compare by subtracting and updating flags; an alias of `subs` with `xzr`/`wzr` as destination. |
| `tst` | Test bits by ANDing and updating flags; an alias of `ands` with `xzr`/`wzr`. |
| `b` | Unconditional branch. |
| `bl` | Branch with link; calls a function and writes the return address to `x30`. |
| `br` | Branch to an address held in a register. |
| `ret` | Return, normally branching to `x30`. |
| `cbz` / `cbnz` | Compare a register with zero and branch if zero / nonzero. |
| `b.eq`, `b.ne`, ... | Conditional branch based on `nzcv` flags. |
| `nop` | No operation. |
| `wfi` | Wait for interrupt. |
| `dsb` / `dmb` / `isb` | Synchronization barriers for completion, memory ordering, and instruction visibility. |

## Instruction suffixes and aliases

| Term | Meaning |
|---|---|
| `b` in a mnemonic such as `ldrb` | Byte-sized operation, not branch. |
| `h` in `ldrh` | Halfword-sized operation. |
| `s` in `adds` / `subs` | Update condition flags. |
| `.eq`, `.ne`, `.lt`, `.ge`, ... | Condition suffixes for conditional branches and select instructions. |
| `ret` | Alias for `br x30`. |
| `cmp` | Alias for `subs xzr, ...` or `subs wzr, ...`. |
| `mov` | Common alias; the assembler chooses an encoding such as `orr` or `add`. |
| `ldr x0, =symbol` | Pseudo-instruction that materializes a constant or address, potentially using a literal pool. |

## Conditions

| Suffix | Meaning |
|---|---|
| `eq` | Equal (`Z == 1`) |
| `ne` | Not equal (`Z == 0`) |
| `cs` / `hs` | Carry set / unsigned higher or same (`C == 1`) |
| `cc` / `lo` | Carry clear / unsigned lower (`C == 0`) |
| `mi` / `pl` | Minus / plus (`N == 1` / `N == 0`) |
| `vs` / `vc` | Overflow / no overflow (`V == 1` / `V == 0`) |
| `hi` / `ls` | Unsigned higher / lower or same |
| `ge` / `lt` | Signed greater or equal / less than |
| `gt` / `le` | Signed greater than / less or equal |
| `al` | Always; commonly omitted |

## ABI and startup terms

| Term | Meaning |
|---|---|
| AArch64 / ARM64 | The 64-bit execution state of the ARMv8-A and later architectures. |
| AAPCS64 | The procedure-call standard describing argument registers, return values, stack alignment, and saved registers. |
| Caller-saved | Registers a called function may freely overwrite; the caller saves them if needed. |
| Callee-saved | Registers a called function must preserve before returning. |
| Stack alignment | Under AAPCS64, `sp` must be 16-byte aligned at a public interface. |
| Prologue | Instructions at function entry that save registers and allocate a stack frame. |
| Epilogue | Instructions at function exit that restore registers and release the stack frame. |
| MMIO | Memory-mapped I/O: device registers accessed using normal load/store instructions. |
| Exception level (`EL0`-`EL3`) | ARM privilege levels. Higher levels generally have more system privileges. |

## Small example

```asm
.section .text
.global _start
.type _start, %function

_start:
    ldr x0, =stack_top     // Load the address of the stack
    mov sp, x0             // Initialize the stack pointer
    mov x0, xzr            // Set x0 to zero

1:
    wfi                    // Wait for an interrupt
    b 1b                   // Branch to the previous label named 1

.size _start, . - _start
```

Numeric local labels such as `1:` can be referenced with `1b` (the previous
definition) or `1f` (the next definition).
