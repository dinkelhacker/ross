.section ".text"
.globl spin_lock
spin_lock:
	mov	w2, #1
	sevl
l1:	wfe
l2:	msr daifset, #2		/* disable irqs */
	ldaxr	w1, [x0]
	cbnz	w1, l3
	stxr	w1, w2, [x0]
	cbnz	w1, l2
	ret
l3:	msr daifclr, #2		/* enable irqs */
	b	l1

/* No need for sev to unlock since in AArch64 the global monitor for a PE changes from
 * exclusive access to open access an event is issued. (E2.9.6 Use of WFE and SEV 
 * instructions by spin-locks). */
.globl spin_unlock
spin_unlock:
	stlr	wzr, [x0]
	msr daifclr, #2		/* enable irqs */
	ret
