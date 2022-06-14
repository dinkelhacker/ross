#include "io.h"
#include "syscall.h"
#include "memory_layout.h"
#include "mmu_armv8a.h"
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

void transition_process(struct user_transition_ctx *ctx)
{
	uint32_t thiscore = get_core_id();
	processor_state *state =
		(processor_state *) (current[thiscore] + PAGE_SIZE - sizeof(processor_state));
	memzero((void*) state, sizeof(*state));

	uint64_t userspace_code = get_free_upage();
	uint64_t userspace_stack = get_free_upage();
	struct mmap *mmap = mmu_create_user_mapping(userspace_code, userspace_stack);
	mmu_update_user_mapping(mmap);
	// TODO: for now code goes to 0x0 and data goes to 0x1000
	relocate_code(ctx->reloc_start, ctx->reloc_end, 0x0/*userspace_code*/);

	state->pc = (unsigned long) 0x0/*userspace_code*/;
	state->pstate = EL0t;
	//TODO: Error check
	state->sp = /*userspace_stack*/ 0x1000 + PAGE_SIZE;
}
