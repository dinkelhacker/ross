.section ".text"

.globl  jumpToAddr
jumpToAddr:
	br x0

.globl getExceptionLevel
getExceptionLevel:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret
