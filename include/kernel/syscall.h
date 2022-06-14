#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSC_TASK_SUSPEND 	0x10
#define SYSC_UART_PRINT		0x11

#ifndef _ASM_
#include <stdint.h>
inline __attribute__((always_inline)) void syscall(uint32_t id)
{
	__asm__ volatile (
	"mov 	x8, %0\n"
	"svc  	#0x0"
	:: "r" (id));
}


inline __attribute__((always_inline)) void sysc_print(char * text, uint32_t len)
{
	__asm__ volatile (
	"mov	x10, %0\n"
	"mov 	x11, %1\n"
	"ldr	x8, =%2\n"
	"svc	#0x0" :: "r" (text), "r" (len), "i" (SYSC_UART_PRINT));

}
#endif
#endif
