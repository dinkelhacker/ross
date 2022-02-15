#ifndef TASKS_H
#define TASKS_H

#include "types.h"

// offset of the cpu context in the task struct
#define OFFSET_CPU_CONTEXT 0

#ifndef _ASM_
// according to the ARM calling conventions registers 0 - 18 can be overwritten by called functions
// therefore they must not be restored as we can't assume they will survive a function call
struct cpu_context {
	ulong x19;
	ulong x20;
	ulong x21;
	ulong x22;
	ulong x23;
	ulong x24;
	ulong x25;
	ulong x26;
	ulong x27;
	ulong x28;
	ulong fp;
	ulong sp;
	ulong pc;
};

typedef struct task_struct {
	struct cpu_context cpu_context;
	long state;
	long counter;
	long priority;
	long preempt_count;
	long pid;
};


#define TASK_RUNNING 1


extern struct task_struct *current;
extern struct task_struct * task[5];
extern int nr_tasks;

extern void process(char* string);
extern int fork(ulong fn_addr, ulong args);
#endif
#endif
