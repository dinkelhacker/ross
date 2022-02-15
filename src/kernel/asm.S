.section ".text"
.globl ret_from_fork
//.weak schedule_tail
ret_from_fork:
	// enable preemption
	bl    schedule_tail
	// restore argument
	mov    x0, x20
	// bye bye...
	blr    x19


.globl cpu_switch_to
cpu_switch_to:
	mov	x10, 0 //#THREAD_CPU_CONTEXT
	add	x8, x0, x10
	mov	x9, sp
	// store callee-saved registers
	stp	x19, x20, [x8], #16
	stp	x21, x22, [x8], #16
	stp	x23, x24, [x8], #16
	stp	x25, x26, [x8], #16
	stp	x27, x28, [x8], #16
	stp	x29, x9, [x8], #16
	str	x30, [x8] // link register
	
	add	x8, x1, x10
	// restore callee-saved registers
	ldp	x19, x20, [x8], #16
	ldp	x21, x22, [x8], #16
	ldp	x23, x24, [x8], #16
	ldp	x25, x26, [x8], #16
	ldp	x27, x28, [x8], #16
	ldp	x29, x9, [x8], #16
	ldr	x30, [x8]
	mov	sp, x9
	ret
