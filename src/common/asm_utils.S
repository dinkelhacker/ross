.section ".text"
.globl  jumpToAddr
jumpToAddr:
  br x0

.globl getExceptionLevel
getExceptionLevel:
    mrs x0, CurrentEL
    lsr x0, x0, #2
    ret
