#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSC_TASK_SUSPEND 	0x10
#define SYSC_UART_PRINT		0x11

#ifndef _ASM_
#include <stdint.h>
extern void syscall(uint32_t id);
extern void sysc_print(char * text, uint32_t len);
#endif
#endif
