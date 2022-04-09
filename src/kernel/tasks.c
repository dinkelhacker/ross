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
#include "asm_utils.h"
#include "peripherals.h"

#include "exceptions.h"
static volatile uint32_t reset = 0;
static struct task_struct init_task0 =	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1,0,0 };
static struct task_struct init_task1 =	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1,0,0 };
static struct task_struct init_task2 =	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1,0,0 };
static struct task_struct init_task3 =	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1,0,0 };
#define EL0t 0x00000000

struct task_struct *current[4] = {&(init_task0), &(init_task1), &(init_task2), &(init_task3)};
struct task_struct * task[4][5]= {
	{&(init_task0), }, 
	{&(init_task1), }, 
	{&(init_task2), }, 
	{&(init_task3), }, 
};
int nr_tasks[4] = {1,1,1,1};

void process(char *string)
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


void reset_device(void)
{
	while(1) {
		if(reset){
			mmio_write(PM_WDOG, PM_PASSWORD | 1);
			mmio_write(PM_RSTC, PM_PASSWORD | PM_RSTC_RESET);
		}

		delay(1000000);
	}
}

void transition_process(unsigned long fn)
{
	uint32_t thiscore = get_core_id();
	processor_state *state =(processor_state *) (current[thiscore] + PAGE_SIZE - sizeof(processor_state)); // get_pt_processor_state(current);
	memzero((unsigned long) state, sizeof(*state));
	state->pc = fn;
	state->pstate = EL0t; 
	unsigned long userspace_stack = get_free_page();
	//TODO: Error check
	state->sp = userspace_stack + PAGE_SIZE;
}


void suspended(void)
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
