.section ".text"

.globl  jump_to
jump_to:
	br x0

.globl get_el
get_el:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl get_core_id
get_core_id:
	mrs x0, mpidr_el1
	/* Mask out RES1[31] bit. */
	and x0, x0, #3
	ret
