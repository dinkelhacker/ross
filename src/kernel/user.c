#include <stdint.h>
#include <syscall.h>
void user_process(void)
{
	sysc_print("Suspended 1\n", 12);
	syscall(SYSC_TASK_SUSPEND);
	sysc_print("Suspended 2\n", 12);
	syscall(SYSC_TASK_SUSPEND);
	sysc_print("Suspended 3\n", 12);
	volatile int c = 0;
	while(1) {
		if (c == 1000)
			c = 0;
		c++;
	}
}
