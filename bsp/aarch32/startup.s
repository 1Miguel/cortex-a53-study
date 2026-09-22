
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
  mov r0, #0
  mov r1, #0
  mov r2, #0
  mov r3, #0
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
