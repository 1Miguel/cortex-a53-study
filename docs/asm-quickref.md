# ARM 32-bit Instruction Set Quick Reference

| Category | Instruction | Syntax | Operation | Notes |
|---|---|---|---|---|
| Data | `MOV` | `MOV Rd, Op2` | `Rd = Op2` | Move |
| Data | `MVN` | `MVN Rd, Op2` | `Rd = ~Op2` | Move NOT |
| Data | `MOVW` | `MOVW Rd, #imm16` | `Rd[15:0] = imm16` | Move 16-bit immediate |
| Data | `MOVT` | `MOVT Rd, #imm16` | `Rd[31:16] = imm16` | Set upper 16 bits |
| Arithmetic | `ADD` | `ADD Rd, Rn, Op2` | `Rd = Rn + Op2` | Add |
| Arithmetic | `ADC` | `ADC Rd, Rn, Op2` | `Rd = Rn + Op2 + C` | Add with carry |
| Arithmetic | `SUB` | `SUB Rd, Rn, Op2` | `Rd = Rn - Op2` | Subtract |
| Arithmetic | `SBC` | `SBC Rd, Rn, Op2` | `Rd = Rn - Op2 - !C` | Subtract with carry |
| Arithmetic | `RSB` | `RSB Rd, Rn, Op2` | `Rd = Op2 - Rn` | Reverse subtract |
| Arithmetic | `RSC` | `RSC Rd, Rn, Op2` | `Rd = Op2 - Rn - !C` | Reverse subtract with carry |
| Logic | `AND` | `AND Rd, Rn, Op2` | `Rd = Rn & Op2` | Bitwise AND |
| Logic | `ORR` | `ORR Rd, Rn, Op2` | `Rd = Rn \| Op2` | Bitwise OR |
| Logic | `EOR` | `EOR Rd, Rn, Op2` | `Rd = Rn ^ Op2` | XOR |
| Logic | `BIC` | `BIC Rd, Rn, Op2` | `Rd = Rn & ~Op2` | Bit clear |
| Compare | `CMP` | `CMP Rn, Op2` | `Rn - Op2` | Updates `NZCV` |
| Compare | `CMN` | `CMN Rn, Op2` | `Rn + Op2` | Updates `NZCV` |
| Compare | `TST` | `TST Rn, Op2` | `Rn & Op2` | Updates `NZCV` |
| Compare | `TEQ` | `TEQ Rn, Op2` | `Rn ^ Op2` | Updates `NZCV` |
| Multiply | `MUL` | `MUL Rd, Rn, Rm` | `Rd = Rn * Rm` | Multiply |
| Multiply | `MLA` | `MLA Rd, Rn, Rm, Ra` | `Rd = Rn * Rm + Ra` | Multiply accumulate |
| Multiply | `MLS` | `MLS Rd, Rn, Rm, Ra` | `Rd = Ra - Rn * Rm` | Multiply subtract |
| Multiply | `UMULL` | `UMULL RdLo, RdHi, Rn, Rm` | `RdHi:RdLo = Rn * Rm` | Unsigned 32×32→64 |
| Multiply | `UMLAL` | `UMLAL RdLo, RdHi, Rn, Rm` | `RdHi:RdLo += Rn * Rm` | Unsigned accumulate |
| Multiply | `SMULL` | `SMULL RdLo, RdHi, Rn, Rm` | `RdHi:RdLo = Rn * Rm` | Signed 32×32→64 |
| Multiply | `SMLAL` | `SMLAL RdLo, RdHi, Rn, Rm` | `RdHi:RdLo += Rn * Rm` | Signed accumulate |
| Divide | `UDIV` | `UDIV Rd, Rn, Rm` | `Rd = Rn / Rm` | Unsigned |
| Divide | `SDIV` | `SDIV Rd, Rn, Rm` | `Rd = Rn / Rm` | Signed |
| Shift | `LSL` | `LSL Rd, Rn, #n` | `Rd = Rn << n` | Logical left |
| Shift | `LSR` | `LSR Rd, Rn, #n` | `Rd = Rn >> n` | Logical right, zero fill |
| Shift | `ASR` | `ASR Rd, Rn, #n` | `Rd = Rn >> n` | Arithmetic right, sign fill |
| Shift | `ROR` | `ROR Rd, Rn, #n` | Rotate right | Rotate |
| Shift | `RRX` | `RRX Rd, Rn` | Rotate right through `C` | 33-bit rotate |
| Memory | `LDR` | `LDR Rd, [Rn]` | `Rd = *(uint32_t *)Rn` | Load word |
| Memory | `STR` | `STR Rd, [Rn]` | `*(uint32_t *)Rn = Rd` | Store word |
| Memory | `LDRB` | `LDRB Rd, [Rn]` | Load 8-bit unsigned | Load byte |
| Memory | `STRB` | `STRB Rd, [Rn]` | Store 8 bits | Store byte |
| Memory | `LDRH` | `LDRH Rd, [Rn]` | Load 16-bit unsigned | Load halfword |
| Memory | `STRH` | `STRH Rd, [Rn]` | Store 16 bits | Store halfword |
| Memory | `LDRSB` | `LDRSB Rd, [Rn]` | Load signed 8-bit | Sign extend |
| Memory | `LDRSH` | `LDRSH Rd, [Rn]` | Load signed 16-bit | Sign extend |
| Memory | `LDRD` | `LDRD Rt, Rt2, [Rn]` | Load 64 bits | Doubleword |
| Memory | `STRD` | `STRD Rt, Rt2, [Rn]` | Store 64 bits | Doubleword |
| Multiple | `LDMIA` | `LDMIA Rn!, {...}` | Load multiple | Increment after |
| Multiple | `LDMIB` | `LDMIB Rn!, {...}` | Load multiple | Increment before |
| Multiple | `LDMDA` | `LDMDA Rn!, {...}` | Load multiple | Decrement after |
| Multiple | `LDMDB` | `LDMDB Rn!, {...}` | Load multiple | Decrement before |
| Multiple | `STMIA` | `STMIA Rn!, {...}` | Store multiple | Increment after |
| Multiple | `STMIB` | `STMIB Rn!, {...}` | Store multiple | Increment before |
| Multiple | `STMDA` | `STMDA Rn!, {...}` | Store multiple | Decrement after |
| Multiple | `STMDB` | `STMDB Rn!, {...}` | Store multiple | Decrement before |
| Stack | `PUSH` | `PUSH {...}` | Store registers on stack | Commonly `SP` |
| Stack | `POP` | `POP {...}` | Restore registers | Commonly `SP` |
| Branch | `B` | `B label` | `PC = label` | Unconditional branch |
| Branch | `BL` | `BL label` | Call function | Saves return address in `LR` |
| Branch | `BX` | `BX Rm` | `PC = Rm` | Branch/exchange |
| Branch | `BLX` | `BLX Rm` | Call through register | Branch/exchange |
| Branch | `CBZ` | `CBZ Rn, label` | Branch if `Rn == 0` | Thumb |
| Branch | `CBNZ` | `CBNZ Rn, label` | Branch if `Rn != 0` | Thumb |
| Bitfield | `BFC` | `BFC Rd, #lsb, #width` | Clear bit field | Clear bits |
| Bitfield | `BFI` | `BFI Rd, Rn, #lsb, #width` | Insert bit field | Replace bits |
| Bitfield | `UBFX` | `UBFX Rd, Rn, #lsb, #width` | Extract unsigned field | Zero extend |
| Bitfield | `SBFX` | `SBFX Rd, Rn, #lsb, #width` | Extract signed field | Sign extend |
| Bit | `CLZ` | `CLZ Rd, Rm` | Count leading zeros | |
| Byte | `REV` | `REV Rd, Rm` | Reverse 4 bytes | Endian conversion |
| Byte | `REV16` | `REV16 Rd, Rm` | Reverse bytes in halfwords | |
| Byte | `REVSH` | `REVSH Rd, Rm` | Reverse low halfword + sign extend | |
| Extend | `UXTB` | `UXTB Rd, Rm` | Zero extend byte | |
| Extend | `UXTH` | `UXTH Rd, Rm` | Zero extend halfword | |
| Extend | `SXTB` | `SXTB Rd, Rm` | Sign extend byte | |
| Extend | `SXTH` | `SXTH Rd, Rm` | Sign extend halfword | |
| Atomic | `LDREX` | `LDREX Rd, [Rn]` | Load exclusive word | Atomic operations |
| Atomic | `STREX` | `STREX Rd, Rm, [Rn]` | Store exclusive word | `0` = success |
| Atomic | `LDREXB` | `LDREXB Rd, [Rn]` | Exclusive byte load | |
| Atomic | `STREXB` | `STREXB Rd, Rm, [Rn]` | Exclusive byte store | |
| Atomic | `LDREXH` | `LDREXH Rd, [Rn]` | Exclusive halfword load | |
| Atomic | `STREXH` | `STREXH Rd, Rm, [Rn]` | Exclusive halfword store | |
| Atomic | `LDREXD` | `LDREXD Rt, Rt2, [Rn]` | Exclusive doubleword load | |
| Atomic | `STREXD` | `STREXD Rd, Rt, Rt2, [Rn]` | Exclusive doubleword store | |
| Atomic | `CLREX` | `CLREX` | Clear exclusive monitor | |
| Barrier | `DMB` | `DMB` | Data memory barrier | Memory ordering |
| Barrier | `DSB` | `DSB` | Data synchronization barrier | Wait for completion |
| Barrier | `ISB` | `ISB` | Instruction synchronization barrier | Refresh instruction stream |
| System | `MRS` | `MRS Rd, CPSR` | Read status register | |
| System | `MSR` | `MSR CPSR_c, Rn` | Write status register | |
| System | `CPSID` | `CPSID i` | Disable IRQ | |
| System | `CPSIE` | `CPSIE i` | Enable IRQ | |
| Exception | `SVC` | `SVC #imm` | Supervisor call | Exception |
| Debug | `BKPT` | `BKPT #imm` | Breakpoint | Debug |
| Power | `WFI` | `WFI` | Wait for interrupt | |
| Power | `WFE` | `WFE` | Wait for event | |
| Power | `SEV` | `SEV` | Send event | |
| Misc | `NOP` | `NOP` | No operation | |
| Coprocessor | `MRC` | `MRC p#, ..., Rd, ...` | Coprocessor → ARM register | AArch32 |
| Coprocessor | `MCR` | `MCR p#, ..., Rd, ...` | ARM register → coprocessor | AArch32 |
| Coprocessor | `MRRC` | `MRRC p#, ..., Rt, Rt2, ...` | Coprocessor → two registers | 64-bit |
| Coprocessor | `MCRR` | `MCRR p#, ..., Rt, Rt2, ...` | Two registers → coprocessor | 64-bit |
| Coprocessor | `CDP` | `CDP p#, ...` | Coprocessor data processing | AArch32 |

---

## Addressing Modes

| Syntax | Effective Address | Writeback |
|---|---|---|
| `[r0]` | `r0` | No |
| `[r0, #4]` | `r0 + 4` | No |
| `[r0, #-4]` | `r0 - 4` | No |
| `[r0, r1]` | `r0 + r1` | No |
| `[r0, r1, LSL #2]` | `r0 + (r1 << 2)` | No |
| `[r0, #4]!` | `r0 + 4` | Yes |
| `[r0, #-4]!` | `r0 - 4` | Yes |
| `[r0], #4` | `r0` | After access |
| `[r0], #-4` | `r0` | After access |

---

## Condition Codes

| Suffix | Meaning | Condition |
|---|---|---|
| `EQ` | Equal | `Z == 1` |
| `NE` | Not equal | `Z == 0` |
| `CS` / `HS` | Carry set / unsigned ≥ | `C == 1` |
| `CC` / `LO` | Carry clear / unsigned < | `C == 0` |
| `MI` | Minus / negative | `N == 1` |
| `PL` | Plus / positive or zero | `N == 0` |
| `VS` | Overflow | `V == 1` |
| `VC` | No overflow | `V == 0` |
| `HI` | Unsigned higher | `C == 1 && Z == 0` |
| `LS` | Unsigned lower or same | `C == 0 || Z == 1` |
| `GE` | Signed greater/equal | `N == V` |
| `LT` | Signed less-than | `N != V` |
| `GT` | Signed greater-than | `Z == 0 && N == V` |
| `LE` | Signed less/equal | `Z == 1 || N != V` |
| `AL` | Always | Always |

---

## Condition Flag Register

| Flag | Name | Meaning |
|---|---|---|
| `N` | Negative | Result bit 31 is set |
| `Z` | Zero | Result is zero |
| `C` | Carry | Carry generated / no unsigned borrow |
| `V` | Overflow | Signed overflow |

---

## Common Instruction Suffixes

| Suffix | Meaning | Example |
|---|---|---|
| `S` | Update condition flags | `ADDS` |
| `B` | Byte | `LDRB` |
| `H` | Halfword | `LDRH` |
| `SB` | Signed byte | `LDRSB` |
| `SH` | Signed halfword | `LDRSH` |
| `D` | Doubleword / variant | `LDRD` |
| `EQ` | Execute if equal | `MOVEQ` |
| `NE` | Execute if not equal | `BNE` |
| `LT` | Signed less-than | `BLT` |
| `LE` | Signed less/equal | `BLE` |
| `GT` | Signed greater-than | `BGT` |
| `GE` | Signed greater/equal | `BGE` |
| `CS` | Carry set | `BCS` |
| `CC` | Carry clear | `BCC` |

---

## Register Quick Reference

| Register | Alias | Typical Use |
|---|---|---|
| `R0-R3` | — | Arguments, return values, scratch |
| `R4-R11` | — | Callee-saved registers |
| `R12` | `IP` | Intra-procedure scratch |
| `R13` | `SP` | Stack pointer |
| `R14` | `LR` | Link / return address |
| `R15` | `PC` | Program counter |
| `CPSR` | — | Current Program Status Register |

---

## Common Bare-Metal Patterns

| Purpose | Assembly |
|---|---|
| Load peripheral address | `LDR r0, =UART_BASE` |
| Read register | `LDR r1, [r0, #UART_FR]` |
| Write register | `STR r1, [r0, #UART_DR]` |
| Set bit | `ORR r0, r0, #(1 << 5)` |
| Clear bit | `BIC r0, r0, #(1 << 5)` |
| Test bit | `TST r0, #(1 << 5)` |
| Extract field | `UBFX r0, r1, #POS, #WIDTH` |
| Insert field | `BFI r0, r1, #POS, #WIDTH` |
| Function call | `BL function` |
| Function return | `BX lr` |
| Save registers | `PUSH {r4-r7, lr}` |
| Restore registers | `POP {r4-r7, pc}` |
| Disable IRQ | `CPSID i` |
| Enable IRQ | `CPSIE i` |
| Memory ordering | `DMB` |
| Wait for memory completion | `DSB` |
| Synchronize instruction stream | `ISB` |
| Wait for interrupt | `WFI` |
| Supervisor call | `SVC #0` |

---

## Frequently Used Patterns

| Pattern | Code | Meaning |
|---|---|---|
| Set bit | `ORR r0, r0, #(1 << n)` | `r0 |= (1 << n)` |
| Clear bit | `BIC r0, r0, #(1 << n)` | `r0 &= ~(1 << n)` |
| Toggle bit | `EOR r0, r0, #(1 << n)` | `r0 ^= (1 << n)` |
| Test bit | `TST r0, #(1 << n)` | Test bit `n` |
| Increment | `ADD r0, r0, #1` | `r0++` |
| Decrement | `SUB r0, r0, #1` | `r0--` |
| Zero register | `MOV r0, #0` | `r0 = 0` |
| Negate | `RSB r0, r1, #0` | `r0 = -r1` |
| Multiply by 2 | `LSL r0, r1, #1` | `r0 = r1 * 2` |
| Multiply by 4 | `LSL r0, r1, #2` | `r0 = r1 * 4` |
| Divide unsigned by 2 | `LSR r0, r1, #1` | `r0 = r1 / 2` |
| Compare to zero | `CMP r0, #0` | Set flags from `r0` |
| Branch if zero | `BEQ label` | `if (Z) goto label` |
| Branch if nonzero | `BNE label` | `if (!Z) goto label` |

---

## PL011 / MMIO Example

| Operation | C | ARM Assembly |
|---|---|---|
| Read register | `value = uart->FR;` | `LDR r0, [r1, #FR]` |
| Write register | `uart->DR = value;` | `STR r0, [r1, #DR]` |
| Test `TXFF` | `(fr & TXFF) != 0` | `TST r0, #(1 << 5)` |
| Clear field | `reg &= ~MASK;` | `BIC r0, r0, #MASK` |
| Set field | `reg |= MASK;` | `ORR r0, r0, #MASK` |
| Extract field | `(reg & MASK) >> POS` | `UBFX r0, r1, #POS, #WIDTH` |

---

## VFP / Floating Point

| Instruction | Syntax | Operation |
|---|---|---|
| `VADD` | `VADD.F32 S0, S1, S2` | `S0 = S1 + S2` |
| `VSUB` | `VSUB.F32 S0, S1, S2` | `S0 = S1 - S2` |
| `VMUL` | `VMUL.F32 S0, S1, S2` | `S0 = S1 * S2` |
| `VDIV` | `VDIV.F32 S0, S1, S2` | `S0 = S1 / S2` |
| `VSQRT` | `VSQRT.F32 S0, S1` | `S0 = sqrt(S1)` |
| `VMOV` | `VMOV S0, S1` | Move floating-point value |
| `VCMP` | `VCMP.F32 S0, S1` | Floating-point comparison |

---

## Coprocessor Quick Reference

| Coprocessor | Typical Purpose |
|---|---|
| `CP10` | VFP |
| `CP11` | VFP |
| `CP14` | Debug |
| `CP15` | System control |

| Instruction | Direction |
|---|---|
| `MRC` | Coprocessor → ARM register |
| `MCR` | ARM register → coprocessor |
| `MRRC` | Coprocessor → two ARM registers |
| `MCRR` | Two ARM registers → coprocessor |
| `CDP` | Coprocessor operation |

---

## ARM Instruction Categories

| Category | Important Instructions |
|---|---|
| Move | `MOV`, `MVN`, `MOVW`, `MOVT` |
| Arithmetic | `ADD`, `ADC`, `SUB`, `SBC`, `RSB` |
| Logic | `AND`, `ORR`, `EOR`, `BIC` |
| Compare | `CMP`, `CMN`, `TST`, `TEQ` |
| Memory | `LDR`, `STR`, `LDRB`, `STRB`, `LDRH`, `STRH` |
| Multiple Memory | `LDM`, `STM` |
| Stack | `PUSH`, `POP` |
| Multiply | `MUL`, `MLA`, `UMULL`, `SMULL` |
| Divide | `UDIV`, `SDIV` |
| Shift | `LSL`, `LSR`, `ASR`, `ROR`, `RRX` |
| Branch | `B`, `BL`, `BX`, `BLX` |
| Bitfield | `BFC`, `BFI`, `UBFX`, `SBFX` |
| Extension | `UXTB`, `UXTH`, `SXTB`, `SXTH` |
| Atomic | `LDREX`, `STREX`, `CLREX` |
| Barrier | `DMB`, `DSB`, `ISB` |
| System | `MRS`, `MSR`, `CPS` |
| Exception | `SVC`, `BKPT` |
| Power | `WFI`, `WFE`, `SEV` |
| Coprocessor | `MRC`, `MCR`, `MRRC`, `MCRR`, `CDP` |
| Floating Point | `VADD`, `VSUB`, `VMUL`, `VDIV`, `VCMP` |

