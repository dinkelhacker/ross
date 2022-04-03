#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "io.h"
#include "asm_utils.h"
#include "exceptions.h"
#include "scheduler.h"
#include "timer.h"
#include "gic400.h"
#include "procconf.h"
#include "error.h"
#include "tasks.h"
#include "syscall.h"
#include "utils.h"
#include "memory_layout.h"


void os_idle(void);

void main(
	uint64_t dtb_ptr32,
	uint64_t x1,
	uint64_t x2,
	uint64_t x3)
{
	(void) dtb_ptr32;
	(void) x1;
	(void) x2;
	(void) x3;
	
	volatile uint32_t exceptionLevel = getExceptionLevel();
	init_vector_table();
 
	// init the interrupt controller
	gic400_init((void *) 0xFF840000UL);
	
	// configure Processor
	_conf_sctlr_el1();
	_conf_hcr_el2();

	// if running at EL3 configure secure configuration register
	if (exceptionLevel == 3) {
		_conf_scr_el3();
		_conf_spsr_el3();
	} else if(exceptionLevel == 2) {
		_conf_spsr_el2(); 
	} else {
		while(1);
	}
}

void os_entry()
{
	/* Init peripherals, system timer and enable IRQs */
	uart_init();
	
	/* Generate interrupt if pin 16 is `high` (for "reset pin"). */
	(void) gpio_set_pull(16, GPIO_P_DOWN);
	(void) gpio_function_select(16, GPIO_F_IN);
	(void) gpio_high_level_detect(16,1);

	timer_init();
	enable_irq();

	uart_writeText("Kernel running! \n");
	print_current_el();
	print_core_id();
	volatile int stop = 1;
	while(stop){};
	uint32_t *p = CORE_RELEASE;
	*p =0xabcdef;

	/* Fork some kernel tasks. */
	(void) fork((unsigned long) &process, (unsigned long) "Task 1\n");
	(void) fork((unsigned long) &process, (unsigned long) "Task 2\n");
	/* User space task. */
	(void) fork((unsigned long) &transition_process,(unsigned long) &suspended);

	/* Go to the os idle loop. */
	os_idle();
}

void os_idle()
{
	while(1) {
		uart_writeText("OS Idle\n");
		delay(1000000);
	}
}


