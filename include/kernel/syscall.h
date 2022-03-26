#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

#define SYSC_TASK_SUSPEND 0x10

extern void syscall(uint32_t id);
#endif
