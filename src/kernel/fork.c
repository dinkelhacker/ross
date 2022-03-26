#include "error.h"
#include <stdint.h>
#include "tasks.h"
#include "memory_layout.h"
#include "scheduler.h"
#include "mm.h"

extern void ret_from_fork(void);

#define THREAD_SIZE 4096

int fork(unsigned long fn_addr, unsigned long args)
{
	
	// disable preemption during the fork
	disable_preemption();

	//allocate a new task struct
	task_struct *p = (task_struct *) get_free_page();
	if(!p)
		return ERROR;

	// copy priority of current task
	p->priority = current->priority;
	p->state    = TASK_RUNNING;
	p->counter  = p->priority;

	// set the task's preempt counter so it won't be interrupted during the
	// end of the schedule process
	p->preempt_count = 1;
	
	// setup cpu_context of forked task
	p->cpu_context.x19 = fn_addr;
	p->cpu_context.x20 = args;

	// setup pc so we will immediately execute the schedule tail
	// and return form fork
	p->cpu_context.pc = (unsigned long) ret_from_fork;
	p->cpu_context.sp = (unsigned long) p + PAGE_SIZE;

	int pid = nr_tasks++;
	p->pid = pid;
	task[pid] = p;

	
	enable_preemption();
	
	return OK;

}
