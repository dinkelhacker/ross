#define _GNU_SOURCE
#include "io.h"
#include "syscall.h"
#include "memory_layout.h"
#include "error.h"
#include "tasks.h"
#include "timer.h"
#include "utils.h"
#include <stdint.h>
#include "mm.h"

#include "exceptions.h"

static struct task_struct init_task =	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1,0,0 };
#define EL0t 0x00000000

struct task_struct *current = &(init_task);
struct task_struct * task[5]= {&(init_task), };
int nr_tasks = 1;

void
process(char *string)
{
	uint32_t i = 0;

	while(1) {
		uart_writeText(string);
		print_current_el();

		for (uint32_t j=0; j<i; j++) {
			uart_writeText("X");
		}

		uart_writeText("\n");
		i++;
		delay(1000000);
	}
}

void 
transition_process(unsigned long fn)
{
	processor_state *state =(processor_state *) (current + PAGE_SIZE - sizeof(processor_state)); // get_pt_processor_state(current);
	memzero((unsigned long) state, sizeof(*state));
	state->pc = fn;
	state->pstate = EL0t; 
	unsigned long userspace_stack = get_free_page();
	//TODO: Error check
	state->sp = userspace_stack + PAGE_SIZE;
}


void
suspended(void)
{
	uart_writeText("Suspended 1\n");
	syscall(SYSC_TASK_SUSPEND);
	uart_writeText("Suspended 2\n");
	syscall(SYSC_TASK_SUSPEND);
	uart_writeText("Suspended 3\n");

	while(1) {
		//syscall(SYSC_TASK_SUSPEND);
		delay(1000000);
	}
}
