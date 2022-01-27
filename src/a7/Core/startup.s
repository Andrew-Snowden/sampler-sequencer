.syntax unified
.cpu cortex-a7

.equ UART4_TDR, 0x40010028

.equ MODE_FIQ, 0x11
.equ MODE_IRQ, 0x12
.equ MODE_SVC, 0x13
.equ MODE_ABT, 0x17
.equ MODE_UND, 0x1B
.equ MODE_SYS, 0x1F

.section .vector_table, "x"
.global _Reset
.global _start
_Reset:
	b Reset_Handler
	b .
	b .
	b .
	b .
	b .
	b .
	b .

Abort_Exception:
	b .

.section .text
Reset_Handler:
	cpsid if
	
	msr cpsr_c, MODE_FIQ
	ldr sp, =_fiq_stack_end

	msr cpsr_c, MODE_IRQ
	ldr sp, =_irq_stack_end

	msr cpsr_c, MODE_SVC
	ldr sp, =_svc_stack_end

	msr cpsr_c, MODE_SYS
	ldr sp, =_user_stack_end

	mov r0, #0
	ldr r1, =_bss_start
	ldr r2, =_bss_end

bss_loop:
	cmp r1, r2
	strlt r0, [r1], #4
	blt bss_loop
	
	ldr r0, =_text_end
	ldr r1, =_data_start
	ldr r2, =_data_end
	
data_loop:
	cmp r1, r2
	ldrlt r3, [r0], #4
	strlt r3, [r1], #4
	blt data_loop

	ldr r4, =UART4_TDR
	mov r0, #65
	str r0, [r4]

	CPSIE i
	
	bl main
	b .
