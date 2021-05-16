#include "tasks.h"
#include "io.h"
#include <stdbool.h>
#include <tasks.h>
#include <scheduler.h>


void switch_to(struct task_struct * next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);



void scheduler(){
  uart_writeText("scheduler\n");
    switch_to(task[(current->pid + 1) % nr_tasks]);
}

void schedule_tail(){
  enable_preemption();
}


void enable_preemption(){
  current->preempt_count--;
}

void disable_preemption(){
  current->preempt_count++;
}

void switch_to(struct task_struct * next) 
{
	if (current == next) 
		return;
	struct task_struct * prev = current;
	current = next;
	cpu_switch_to(prev, next);
}
