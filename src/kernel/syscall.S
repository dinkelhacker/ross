.section ".text"
/*
 * Syscall numbers goes into x8 ?
 *
 * void syscall(uint id)
 */
 .globl syscall

 // use immediate? (goes into ESR)
syscall:
	mov x8, x0 
	svc  #0x0
	ret
