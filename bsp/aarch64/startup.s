.section .text.startup
.global _reset

/*
 * @brief When saving register, we save is in stack, the `stp` (store pair)
 * instruction says that take 2 pair of registers, and store them in memory
 * in this case, address dictated by stack pointer.
 *
 * The [sp, #-0x10]! sp with offset -16 (yes negative, because the stack
 * grows downward), 16 since a 2 pair of register is
 *  (2 x 64-bits = 128-bits = 16bytes)
 *
 * The '!' at the end means, write-back sp - 16 permanently, this is equivalent
 * in C to:
 *
 *  *(sp - 16) = x0 | (x1 << 64);
 *  sp -= 16;
 */
.macro saveregister
	stp X0,X1,   [sp,#-0x10]!
	stp X2,X3,   [sp,#-0x10]!
	stp X4,X5,   [sp,#-0x10]!
	stp X6,X7,   [sp,#-0x10]!
	stp X8,X9,   [sp,#-0x10]!
	stp X10,X11, [sp,#-0x10]!
	stp X12,X13, [sp,#-0x10]!
	stp X14,X15, [sp,#-0x10]!
	stp X16,X17, [sp,#-0x10]!
	stp X18,X19, [sp,#-0x10]!
	stp X29,X30, [sp,#-0x10]!
.endm

/* restore the register states by popping the values from stack
 * then permanently increment the stack pointer, in c this is
 * equivalent to:
 *
 *  x0 = *(sp + 0);
 *  x1 = *(sp + 8);
 *  sp += 16;
*/
.macro restoreregister
	ldp X29,X30, [sp], #0x10
	ldp X18,X19, [sp], #0x10
	ldp X16,X17, [sp], #0x10
	ldp X14,X15, [sp], #0x10
	ldp X12,X13, [sp], #0x10
	ldp X10,X11, [sp], #0x10
	ldp X8,X9,   [sp], #0x10
	ldp X6,X7,   [sp], #0x10
	ldp X4,X5,   [sp], #0x10
	ldp X2,X3,   [sp], #0x10
	ldp X0,X1,   [sp], #0x10
.endm

// **************************************************************************
_reset:
  // initialise all registers
  //
  // ARM processors use some non-reset flip-flops. This can cause 
  // X-propagation issues in simulations. Register initialization helps
  // reduce the possibility of the issue
  //
  // NOTE: This is not required in actual hardware/silicon
  //
  // ... you might notice `xzr`, xzr is a special register
  // that is always zero (hardcode to 0).
  mov x0,  xzr
  mov x1,  xzr
  mov x2,  xzr
  mov x3,  xzr
  mov x4,  xzr
  mov x5,  xzr
  mov x6,  xzr
  mov x7,  xzr
  mov x8,  xzr
  mov x9,  xzr
  mov x10, xzr
  mov x11, xzr
  mov x12, xzr
  mov x13, xzr
  mov x14, xzr
  mov x15, xzr
  mov x16, xzr
  mov x17, xzr
  mov x18, xzr
  mov x19, xzr
  mov x20, xzr
  mov x21, xzr
  mov x22, xzr
  mov x23, xzr
  mov x24, xzr
  mov x25, xzr
  mov x26, xzr
  mov x27, xzr
  mov x28, xzr
  mov x29, xzr
  mov x30, xzr

  // get the current exception level
  // the EL field is bits [3:2] so we shift and apply mask
  // we will not use this, we only want to watch and check
  // the current exception level
  mrs x3, currentEL
  lsr x3, x3, #0x2
  and x3, x3, #0x3

  // get the cpu id via MIPDR_EL1, Multiprocessor Affinity Registers
  // the MPIDR provides processor identification
  // See AArch64 Reference Manual, D13.2.99 MIPDR_EL1
  mrs x1, MPIDR_EL1
  // the lsb bytes contains the cpu id
  and x1, x1, 0xFF
  // initialise stack ptr depending on the processor
  cmp x1, 0
  beq _init_cpu0

_init_cpu0:
  // ... the ldr instruction below is a "pseudo instruction" it
  // tells to load the `stack_top` address we defined in the linker
  // script into regixter x30.
  //
  // ... ok, to clarify even further, when code access symbols defined
  // in the linker script, the value of the symbol is treated as address
  // think of it as `stack_top` is a var but the address of the var contains
  // the value you assigned to the symbol `stack_top` in linker script.
  ldr x0, =__stack_top_el3

  //  Now the x30 holds the address value defined by linker symbol
  //  stack_top. set the value of the stack pointer to x30
  //
  //  TODO: each cpu must have its own stack pointer starting
  //  address, for now we only use cpu0
  mov sp, x0

  // load el3_vtable mem address to register x1
  ldr x1, =_exc_vector_table

  // assign x1 (address of el3_vtable) to EL3 Vector Table
  // Base Address Register (VBAR)
  msr VBAR_EL3, x1

  // initialise gic and route to non-secure mode
  mrs x0, ICC_SRE_EL3
  orr x0, x0, #(1 << 3) // Enable lower EL access to system register
  orr x0, x0, #(1 << 2) // Disable FIQ bypass
  orr x0, x0, #(1 << 1) // Disable IRQ bypass
  orr x0, x0, #(1 << 0) // System reg access at EL3 enable
  msr ICC_SRE_EL3, x0
  isb

  // disable secure state GICD.DS
  ldr x0, =__gicd_base
  ldr w1, [x0]
  orr w1, w1, #(1U << 6) // disable secure state GICD_CTLR.DS 
  orr w1, w1, #(1U << 5) // enable GICD_CTLR.ARE_NS
  orr w1, w1, #(1U << 4) // enable GICD_CTLR.ARE_S
  str w1, [x0]
  

  /* disable timer and masked (disable) interrupt */
  mov x0, 0x02
  msr CNTP_CTL_EL0, x0
  /* set the timer frequency (armv8, cortex-a53) */
  ldr x0, =62500000
  msr CNTFRQ_EL0, x0
                   
// **************************************************************************
_el3_init:
	mrs x0, scr_el3
  // 64-bit mode
	orr x0, x0, #(1 << 10)
  // non secure state
	orr x0, x0, #(1 << 0)
  // route IRQ exceptions to EL1, not EL3
	bic x0, x0, #(1 << 1)
  // we will disable SCR.HCE bit which disable hypervisor (EL2)
  // and x0, x0, #(~(0x01 << 8))
  // write x0 to scr
	msr scr_el3, x0
  isb

  // HCR controls the EL1 execution
  mrs x0, hcr_el2
  orr x0, x0, #(1 << 31)
  // Do not route physical IRQ exceptions from EL1 to EL2
	bic x0, x0, #(1 << 4)
  msr hcr_el2, x0
  isb

  // at this point we will go to el1
	msr sctlr_el1, xzr

  // we will now set the spsr to EL1 so when
  // eret is invoked, the PSTATE will be restored to SPSR
	mov x0, #0x5
	msr spsr_el3, x0
  isb

  // set to el1_entry
	ldr x0, =_el1_entry
	msr elr_el3, x0
  isb

	eret

.balign 0x04
_el1_entry:
  ldr x0, =__stack_top_el1
  mov sp, x0

  ldr x1, =_exc_vector_table
  msr vbar_el1, X1
  isb

  // prevent fip/simd to be trap in exception
  mrs x0, cpacr_el1
  orr x0, x0, #(3 << 20)
  msr cpacr_el1, x0
  isb

// **************************************************************************
_data_load:
  ldr x10, =__data_load_start__
  ldr x11, =__data_start__
  ldr x12, =__data_end__

_data_copy_loop:
  // set data_ram[idx] = data_load[idx]
  ldr x13, [x10]
  str x13, [x11]
  // is &bss[idx] == &bss[end_idx]
  cmp x11, x12
  // if greater or equal, go to end
  bge _data_copy_end
  // else address(aka idx) increment
  // note that we copy by u64, so make sure that the section
  // alignment in the linker script is aligned by 8, otherwise
  // we might hit beyond the end of the section boundary
  add x11, x11, #8
  add x10, x10, #8
  // keep copying data segment
  b _data_copy_loop

_data_copy_end:
// **************************************************************************

_bss_clear:
  ldr x28, =__bss_start__
  ldr x29, =__bss_end__
_bss_loop:
  // set bss[idx]=0
  str xzr, [x28]
  // is &bss[idx] == &bss[end_idx]
  cmp x28, x29
  // if greater or equal, go to end
  bge _bss_end
  // else address(aka idx) increment
  add x28, x28, #4
  // keep clearing bss mem
  b _bss_loop

_bss_end:
  // call the libc initialization routines
  // bl __libc_init_array
  // Initialise the GIC before allowing interrupts to reach the CPU.
  bl gic_v3_init

  // enable irqs before jumping to main
  msr daifclr, #3
  // jump to main
  bl main
  b .

// **************************************************************************
// When an excption occurs, the processor execute handler codes
// the processor will look into the vector table and will execute
// handler depending on the exception type. Refer to 
//
// ARM V8 Architectur Programmers Guide
// AArch64 Exception Handling
// Table 10-2 Vector table offsets from vector table base address
//
// to know the address of each entries that will be executed depending
// on the type of exception.
//
// The type of exception depends on:
//  1. where the exception is taken
//  2. the stack pointer that is being used (PSTATE.SP).
//
// Example
//  when the current exception level is EL1 and IRQ exception occurs at EL1,
//  and the stack pointer being used is SP_EL1, then the exception vector
//  offset to be used is VBAR + 0x80 (IRQ).
//
// NOTE that each entry in the exception table consist of 16 exception
// handler. Each exception handler is *32 instructions* in size.
// *1 instruction* is 4-bytes, so *32 instructions* is 128-bytes (0x80).
// this means a vector table entry is 128-bytes (0x80) long, so it also
// has to be 128-byte(0x80) aligned.
//
// ensure 2kiB alignment
.balign 0x800
_exc_vector_table:
  // functions prefix with sp0 are functions that will be
  // executed if exception is taken with SP0 (SP_ELn Thread
  // mode) used - (PSTATE.SP == 0)
  b _sp0_sync_handler
  .balign 0x80 // ensure 128-byte alignment
  b _sp0_irq_handler
  .balign 0x80
  b _sp0_fiq_handler
  .balign 0x80
  b _sp0_serror_handler
  // functions prefix with sp0 are functions that will be
  // executed if exception is taken with SPn (SP_ELn Handler
  // mode) used - (PSTATE.SP == 0)
  .balign 0x80
  b _spx_sync_handler
  .balign 0x80
  b _spx_irq_handler
  .balign 0x80
  b _spx_fiq_handler
  .balign 0x80
  b _spx_serror_handler
  // exceptions executed when an exception occurs from lower
  // exception level, i.e in el3 and exception from el2(if implemented)
  // or el1 (if el2 not implemented)
  .balign 0x80
  b _low_el_sync_handler
  .balign 0x80
  b _low_el_irq_handler
  .balign 0x80
  b _low_el_fiq_handler
  .balign 0x80
  b _low_el_serror_handler
  // exceptions executed when an exception occurs from lower
  // excecution state (aarch32)
  .balign 0x80
  b _a32_sync_handler
  .balign 0x80
  b _a32_irq_handler
  .balign 0x80
  b _a32_fiq_handler
  .balign 0x80
  b _a32_serror_handler

// **************************************************************************
// Below are all exception routines
// **************************************************************************
_sp0_sync_handler:
  // when entering an exception handler, the CPU state PSTATE register
  // is stored in SPSR (Saved Processor State Register).
  //
  // but we have to manually save the general-purpose registers in stack
  saveregister

  bl el_sp0_sync_handler

  // manually restore the general-purpose registers from stack
  restoreregister

  // Ending exception handling and returning to the previous Exception
  // level is performed by executing the ERET instruction.
  //
  // This causes the SPSR_ELn to be copied into PSTATE.
  //
  // This also restores the ALU flags, execution state, Exception level,
  // and the processor branches. From here, you continue execution from
  // the address in ELR_ELn.
  eret
_sp0_irq_handler:
  saveregister
  bl gic_v3_irq_handler
  restoreregister
  eret
_sp0_fiq_handler:
  saveregister
  bl el_sp0_fiq_handler
  restoreregister
  eret
_sp0_serror_handler:
  saveregister
  bl el_sp0_serror_handler
  restoreregister
  eret

_spx_sync_handler:
  saveregister
  mrs x12, currentEL
  bl el_spx_sync_handler
  restoreregister
  eret
_spx_irq_handler:
  saveregister
  bl gic_v3_irq_handler
  restoreregister
  eret
_spx_fiq_handler:
  saveregister
  bl el_spx_fiq_handler
  restoreregister
  eret
_spx_serror_handler:
  saveregister
  bl el_sp0_serror_handler
  restoreregister
  eret

_low_el_sync_handler:
_low_el_irq_handler:
_low_el_fiq_handler:
_low_el_serror_handler:
  mrs x0, currentEL
  b .

_a32_sync_handler:
_a32_irq_handler:
_a32_fiq_handler:
_a32_serror_handler:
  b .
