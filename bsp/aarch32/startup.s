/**********************************************************************
 * @file  startup.s
 * @brief aarch32 arm 32-bit startup code.
 *
 * @note  currently only tested running cortex-A9.
 *
 * @author Miguel Ugsimar
 **********************************************************************/

.equ CPSR_MODE_USR, 0x10
.equ CPSR_MODE_FIQ, 0x11
.equ CPSR_MODE_IRQ, 0x12
.equ CPSR_MODE_SVC, 0x13
.ifdef SECURITY_EXT
.equ CPSR_MODE_MON, 0x16
.endif // SECURITY_EXT
.equ CPSR_MODE_ABT, 0x17
.equ CPSR_MODE_UND, 0x1B
.equ CPSR_MODE_SYS, 0x1F
.equ CPSR_MODE_MSK, 0x1F

.global _reset
.section .text.startup

_reset:
  b reset_handler
  b undefined_handler
  b swi_handler
  b prefetch_handler
  b data_handler
  nop
  b irq_handler
  b fiq_handler

reset_handler:

  // set the each mode stack pointer
  ldr sp, =__cpu0_stack_top_svc
 
  mrs r0, cpsr
	and r0, r0, #(~CPSR_MODE_MSK)
	orr r0, r0, #CPSR_MODE_IRQ
	msr cpsr, r0
  ldr sp, =__cpu0_stack_top_irq

  mrs r0, cpsr
	and r0, r0, #(~CPSR_MODE_MSK)
	orr r0, r0, #CPSR_MODE_FIQ
	msr cpsr, r0
  ldr sp, =__cpu0_stack_top_fiq

  mrs r0, cpsr
	and r0, r0, #(~CPSR_MODE_MSK)
	orr r0, r0, #CPSR_MODE_ABT
	msr cpsr, r0
  ldr sp, =__cpu0_stack_top_abt

.ifdef SECURITY_EXT
  mrs r0, cpsr
	and r0, r0, #(~CPSR_MODE_MSK)
	orr r0, r0, #CPSR_MODE_MON
	msr cpsr, r0
  ldr sp, =__cpu0_stack_top_mon
.endif // SECURITY_EXT

  mrs r0, cpsr
	and r0, r0, #(~CPSR_MODE_MSK)
	orr r0, r0, #CPSR_MODE_UND
	msr cpsr, r0
  ldr sp, =__cpu0_stack_top_und

  mrs r0, cpsr
	and r0, r0, #(~CPSR_MODE_MSK)
	orr r0, r0, #CPSR_MODE_SYS
	msr cpsr, r0
  ldr sp, =__cpu0_stack_top_sys

_data_load:
  ldr r0, =__data_load_start__
  ldr r1, =__data_start__
  ldr r2, =__data_end__
_data_copy_loop:
  ldr r3, [r0]
  str r3, [r1]
  cmp r1, r2
  bge _data_copy_end
  add r1, r1, #4
  add r0, r0, #4
  b _data_copy_loop
_data_copy_end:

_bss_clear:
  mov r4, #0 
  ldr r5, =__bss_start__
  ldr r6, =__bss_end__
_bss_loop:
  str r4, [r5]
  cmp r5, r6
  bge _bss_end
  add r5, r5, #4
  b _bss_loop
_bss_end:
  // call the libc initialization routines
  //bl __libc_init_array
  // jump to main
  bl main
  b .


undefined_handler:
  b .

swi_handler:
  b .

prefetch_handler:
  b .

data_handler:
  b .

irq_handler:
  b .

fiq_handler:
  b .
