.section ".text"
.globl ret_from_fork

#include "tasks.h"

ret_from_fork:
	// enable preemption
	bl    schedule_tail
	// restore argument
	mov    x0, x20
	// bye bye...
	blr    x19


/* 
 * This function performs the actual context switch.
 *
 * x0 = `current` task stuct
 * x1 = `next` task struct
 * 
 * Reminder:
 * [stp|ldp] xA, xb, [xD], #16 
 *	store/load register xA and xB in xD and move pointer in xD by 16
 *
 */
.globl cpu_switch_to
cpu_switch_to:
	mov	x10, #OFFSET_CPU_CONTEXT 

	// get a pointer to the `cpu_context` in the `current` task struct
	add	x8, x0, x10

	// store registers of current task in `current->cpu_context`
	mov	x9, sp
	stp	x19, x20, [x8], #16
	stp	x21, x22, [x8], #16
	stp	x23, x24, [x8], #16
	stp	x25, x26, [x8], #16
	stp	x27, x28, [x8], #16
	stp	x29, x9, [x8], #16
	str	x30, [x8] // link register
	
	// get a pointer to the `cpu_context` in the `next` task struct
	add	x8, x1, x10

	// restore registers from `next->cpu_context`
	ldp	x19, x20, [x8], #16
	ldp	x21, x22, [x8], #16
	ldp	x23, x24, [x8], #16
	ldp	x25, x26, [x8], #16
	ldp	x27, x28, [x8], #16
	ldp	x29, x9, [x8], #16
	ldr	x30, [x8]
	mov	sp, x9
	ret
