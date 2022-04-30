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
#include "spinlock.h"

static spinlock_t print_lock;

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
		spin_lock(&print_lock);
		uart_print(string);

		for (uint32_t j=0; j<i; j++) {
			uart_print("X");
		}

		uart_print("\n");
		spin_unlock(&print_lock);
		i++;
		delay(1000000);
	}
}

void transition_process(unsigned long fn)
{
	uint32_t thiscore = get_core_id();
	processor_state *state =
		(processor_state *) (current[thiscore] + PAGE_SIZE - sizeof(processor_state));
	memzero((void*) state, sizeof(*state));
	state->pc = fn;
	state->pstate = EL0t; 
	unsigned long userspace_stack = get_free_page();
	//TODO: Error check
	state->sp = userspace_stack + PAGE_SIZE;
}

void suspended(void)
{
	uart_print("Suspended 1\n");
	syscall(SYSC_TASK_SUSPEND);
	uart_print("Suspended 2\n");
	syscall(SYSC_TASK_SUSPEND);
	uart_print("Suspended 3\n");

	while(1) {
		delay(1000000);
	}
}
