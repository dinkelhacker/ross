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
#include "mm.h"

static cpu_boot_status *cores = BOOT_CORE_STATUS;

void os_idle(void);

void init(
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

	// init the gic distributor only once from core 0
	if(0 == get_core_id()) {
		gic400_init((void *) 0xFF840000UL);
		gic400_enable_cpuif();
	} else
		gic400_enable_cpuif();
	
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

	enable_irq();

	uart_writeText("Kernel running! \n");
	print_current_el();
	print_core_id();

	/* Fork some kernel tasks. */
	(void) fork((unsigned long) &process, (unsigned long) "Task 1\n", 0);
	(void) fork((unsigned long) &process, (unsigned long) "Task 2\n", 1);
	(void) fork((unsigned long) &process, (unsigned long) "Task 3\n", 2);
	/* User space task. */
	(void) fork((unsigned long) &transition_process,(unsigned long) &suspended, 0);

	/* Release other cores */
	memzero(cores, 3 * sizeof(cpu_boot_status));
	cores->addr = 0x160000;
	cores->core_released = CORE_RELEASED;
	wakeup_cores();
	uart_writeText("Kernel running! \n");
	delay(1000000);
	(cores + 1)->addr = 0x160000;
	(cores + 1)->core_released = CORE_RELEASED;
	wakeup_cores();
	delay(1000000);
	/* Go to the os idle loop. */

	timer_init();
	os_idle();
}

void os_entry_secondary() {
	enable_irq();
	os_idle();
}

void os_idle()
{
	while(1) {
		uart_writeText("OS Idle\n");
		delay(1000000);
	}
}


