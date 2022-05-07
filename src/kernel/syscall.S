#include "syscall.h"
.section ".text"
/*
 * Syscall numbers goes into x8 ?
 *
 * void syscall(uint id)
 */

 // use immediate? (goes into ESR)
.globl syscall
syscall:
	mov x8, x0 
	svc  #0x0
	ret


// void sysc_print(char * text, uint32_t len);
.globl sysc_print
sysc_print:
	mov 	x10, x0
	mov 	x11, x1
	ldr	x0, =SYSC_UART_PRINT
	bl	syscall
	ret
