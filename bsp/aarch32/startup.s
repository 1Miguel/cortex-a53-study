/**********************************************************************
 * @file  startup.s
 * @brief aarch32 arm 32-bit startup code.
 *
 * @note  currently only tested running cortex-A9.
 *
 * @author Miguel Ugsimar
 **********************************************************************/

 // max number of cache level
.equ CACHE_MAX_CACHE_LVL, (7)

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

.equ SCTLR_MMU_ENABLE,     (1 << 0)
.equ SCTLR_I_CACHE_ENABLE, (1 << 12)
.equ SCTLR_D_CACHE_ENABLE, (1 << 2)


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

  /* disable mmu and all caches */
  mrc p15, 0, r1, c1, c0, 0
  bic r1, r1, #SCTLR_MMU_ENABLE
  bic r1, r1, #SCTLR_D_CACHE_ENABLE
  bic r1, r1, #SCTLR_I_CACHE_ENABLE
  mcr p15, 0, r1, c1, c0, 0

  /* invalidate all caches and TLB */
  mov	r0, #0
	mcr	p15, 0, r0, c7, c5, 0		// invalidate instruction cache
	mcr	p15, 0, r0, c7, c5, 6		// Invalidate branch predictor array
	mcr	p15, 0, r0, c8, c7, 0		// invalidate entire unified TLB
  isb
  bl invalidate_dcache

  /* enable all caches */
  mrc p15, 0, r1, c1, c0, 0
  orr r1, r1, #SCTLR_D_CACHE_ENABLE
  orr r1, r1, #SCTLR_I_CACHE_ENABLE
  mcr p15, 0, r1, c1, c0, 0

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

invalidate_dcache:
	mrc	p15, 1, r0, c0, c0, 1		/* read CLIDR */
	ands	r3, r0, #0x7000000
	mov	r3, r3, lsr #23			    /* cache level value (naturally aligned) */
	beq	finished
	mov	r10, #0				          /* start with level 0 */
loop1:
	add	r2, r10, r10, lsr #1		/* work out 3xcachelevel */
	mov	r1, r0, lsr r2			    /* bottom 3 bits are the Cache type for this level */
	and	r1, r1, #7			        /* get those 3 bits alone */
	cmp	r1, #2
	blt	skip				            /* no cache or only instruction cache at this level */
	mcr	p15, 2, r10, c0, c0, 0	/* write the Cache Size selection register */
	isb                         /* isb to sync the change to the CacheSizeID reg */
	mrc	p15, 1, r1, c0, c0, 0		/* reads current Cache Size ID register */
	and	r2, r1, #7			        /* extract the line length field */
	add	r2, r2, #4			        /* add 4 for the line length offset (log2 16 bytes) */
	ldr	r4, =0x3ff
	ands	r4, r4, r1, lsr #3		/* r4 is the max number on the way size (right aligned) */
	clz	r5, r4				          /* r5 is the bit position of the way size increment */
	ldr	r7, =0x7fff
	ands	r7, r7, r1, lsr #13		/* r7 is the max number of the index size (right aligned) */
loop2:
	mov	r9, r4				          /* r9 working copy of the max way size (right aligned) */
loop3:
	orr	r11, r10, r9, lsl r5		/* factor in the way number and cache number into r11 */
	orr	r11, r11, r7, lsl r2		/* factor in the index number */
	mcr	p15, 0, r11, c7, c6, 2	/* invalidate by set/way */
	subs	r9, r9, #1			      /* decrement the way number */
	bge	loop3
	subs	r7, r7, #1			      /* decrement the index */
	bge	loop2
skip:
	add	r10, r10, #2			      /* increment the cache number */
	cmp	r3, r10
	bgt	loop1
finished:
	mov	r10, #0				          /* switch back to cache level 0 */
	mcr	p15, 2, r10, c0, c0, 0	/* select current cache level in cssr */
	dsb
	isb
	bx	lr

undefined_handler:
  b .

swi_handler:
  b .

prefetch_handler:
  b .

data_handler:
  b .

irq_handler:
  stmdb sp!, {r0-r3, r12, lr} /* save state to stack */
  b .
  ldmia sp!, {r0-r3, r12, lr} /* save state to stack */
  subs pc, lr, #4             /* yield execution back to pc - 4 */

fiq_handler:
  b .
