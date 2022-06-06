#ifndef TASKS_H
#define TASKS_H

// offset of the cpu context in the task struct
#define OFFSET_CPU_CONTEXT 0

#ifndef _ASM_
#include <stdint.h>
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

struct mmap {
	uint64_t *codep;
	uint64_t *data;
};

struct user_transition_ctx {
	uint64_t *reloc_start;
	uint64_t *reloc_end;
};

typedef struct task_struct {
	struct cpu_context cpu_context;
	long state;
	long counter;
	long priority;
	long preempt_count;
	long pid;
} task_struct;

// TODO: move?
/* Full processor state as it is expected during kernel enter/exit. */
typedef struct processor_state {
	unsigned long x[31]; /* Registers x0 - x30 */
	unsigned long sp;
	unsigned long pc;
	unsigned long pstate;
} processor_state;


#define TASK_RUNNING 1


extern struct task_struct *current[4];
extern struct task_struct * task[4][5];
extern int nr_tasks[4];

extern void process(char* string);

extern void transition_process(struct user_transition_ctx *ctx);

extern void user_process(void);

extern void reset_device(void);

extern int fork(unsigned long fn_addr, unsigned long args, uint32_t target_core);
#endif
#endif
