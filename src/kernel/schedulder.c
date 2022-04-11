#include "tasks.h"
#include "io.h"
#include <stdbool.h>
#include "tasks.h"
#include "scheduler.h"
#include "asm_utils.h"

void switch_to(struct task_struct * next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);


void scheduler()
{
	uint32_t thiscore = get_core_id();
	uart_print("scheduler\n");
	switch_to(task[thiscore][(current[thiscore]->pid + 1) % nr_tasks[thiscore]]);
}

void schedule_tail()
{
	enable_preemption();
}

void enable_preemption()
{
	uint32_t thiscore = get_core_id();
	current[thiscore]->preempt_count--;
}

void disable_preemption()
{
	uint32_t thiscore = get_core_id();
	current[thiscore]->preempt_count++;
}

void switch_to(struct task_struct * next) 
{
	uint32_t thiscore = get_core_id();
	if (current[thiscore] == next) 
		return;
	struct task_struct * prev = current[thiscore];
	current[thiscore] = next;
	cpu_switch_to(prev, next);
}
