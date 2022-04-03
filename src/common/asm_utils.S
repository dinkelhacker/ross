.section ".text"

.globl  jumpToAddr
jumpToAddr:
	br x0

.globl getExceptionLevel
getExceptionLevel:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl get_core_id
get_core_id:
	mrs x0, mpidr_el1
	/* Mask out RES1[31] bit. */
	and x0, x0, #3
	ret

.globl wakeup_cores
wakeup_cores:
	sev
	ret

.globl memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret
