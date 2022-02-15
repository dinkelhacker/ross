#include "io.h"
#include "syscall.h"
#include "memory_layout.h"
#include "error.h"
#include "tasks.h"
#include "timer.h"

static struct task_struct init_task =	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1,0,0 };

struct task_struct *current = &(init_task);
struct task_struct * task[5]= {&(init_task), };
int nr_tasks = 1;

void process(char* string)
{
	unsigned int i = 0;

	while(1) {
		uart_writeText(string);

		for (int j=0; j<i; j++) {
			uart_writeText("X");
		}

		uart_writeText("\n");
		i++;
		delay(1000000);
	}
}


