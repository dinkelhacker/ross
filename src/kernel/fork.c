#include "error.h"
#include <stdint.h>
#include "tasks.h"
#include "memory_layout.h"
#include "scheduler.h"
#include "mm.h"
#include "asm_utils.h"

extern void ret_from_fork(void);

#define THREAD_SIZE 4096

int fork(unsigned long fn_addr, unsigned long args, uint32_t target_core)
{
	uint32_t thiscore = get_core_id();

	// disable preemption during the fork
	disable_preemption();
	//allocate a new task struct
	task_struct *p = (task_struct *) get_free_virt_kpage();
	if(!p)
		return ERROR;

	// copy priority of current task
	p->priority = current[thiscore]->priority;
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
	/* reserving some space for the processor_state */
	p->cpu_context.sp = (unsigned long) (p + PAGE_SIZE - sizeof(processor_state));
	int pid = nr_tasks[target_core]++;
	p->pid = pid;
	task[target_core][pid] = p;


	enable_preemption();

	return OK;

}
