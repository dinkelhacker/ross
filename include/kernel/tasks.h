#ifndef TASKS_H
#define TASKS_H

// according to the ARM calling conventions registers 0 - 18 can be overwritten by called functions
// therefore they must not be restored as we can't assume they will survive a function call
struct cpu_context {
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
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
extern int fork(unsigned long fn_addr, unsigned long args);
#endif
