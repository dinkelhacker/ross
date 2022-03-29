#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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
	volatile uint32_t exceptionLevel = getExceptionLevel();
	uart_init();
	enable_irq();
	volatile uint32_t stop = 1;
	//while(stop){};
	timer_init();

	uart_writeText("Kernel running! \n");

	if(exceptionLevel == 1)
		uart_writeText("Running at EL1 \n");
	else if(exceptionLevel == 2)
		uart_writeText("Running at EL2 \n");
	else if(exceptionLevel == 3)
		uart_writeText("Running at EL3 \n");
	
	fork((unsigned long) &process, (unsigned long) "Task 1\n");
	fork((unsigned long) &process, (unsigned long) "Task 2\n");
	fork((unsigned long) &transition_process,(unsigned long) &suspended);
	fork((unsigned long) &reset_device, 0ul);
	os_idle();
}

void os_idle()
{
	while(1) {
		uart_writeText("OS Idle\n");
		delay(1000000);
	}
}


