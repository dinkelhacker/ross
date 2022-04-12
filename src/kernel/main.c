#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "io.h"
#include "asm.h"
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
	
	volatile uint32_t exceptionLevel = get_el();
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
	enable_irq();
	
	/* Generate interrupt if pin 16 is `high` (for "reset pin"). */
	(void) gpio_set_pull(16, GPIO_P_DOWN);
	(void) gpio_function_select(16, GPIO_F_IN);
	(void) gpio_high_level_detect(16,1);

	uart_print("Kernel running! \n");
	print_current_el();
	print_core_id();

	/* Fork some kernel tasks. */
	(void) fork((unsigned long) &process, (unsigned long) "Task 1\n", 0);
	(void) fork((unsigned long) &process, (unsigned long) "Task 2\n", 1);
	(void) fork((unsigned long) &process, (unsigned long) "Task 3\n", 2);
	/* User space task. */
	(void) fork((unsigned long) &transition_process,(unsigned long) &suspended, 0);

	/* Release other cores */
	volatile cpu_boot_status *cores = (cpu_boot_status *) BOOT_CORE_STATUS;
	memzero((void *) cores, 3 * sizeof(cpu_boot_status));
	setup_core(&cores[0], 0x160000, CORE_RELEASED);
	setup_core(&cores[1], 0x160000, CORE_RELEASED);
	setup_core(&cores[2], 0x160000, CORE_RELEASED);
	wakeup_cores();
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
		uart_print("OS Idle\n");
		delay(1000000);
	}
}


