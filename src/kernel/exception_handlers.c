#include "exception_types.h"
#include "io.h"
#include "gpio.h"
#include "peripherals.h"
#include "irids.h"
#include "gic400.h"
#include "timer.h"
#include "tasks.h"
#include "scheduler.h"
#include "exceptions.h"
#include "syscall.h"
#include "utils.h"

#define EC_SHIFT 26
#define EC_MASK ((uint8_t) 0x3f)
#define EC_SVC 0x15

static uint8_t get_exception_class(uint32_t esr)
{
	return ((uint8_t)(esr >> EC_SHIFT)) & EC_MASK;	
}

void invalid_exception(uint32_t type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_syscall(uint32_t exception_type, uint32_t sysid)
{
	(void) exception_type;

	switch (sysid){
	case SYSC_TASK_SUSPEND:
		enable_irq();
		uart_print("Syscall\n");
		scheduler();
		disable_irq();
	}
}


void handle_sync(uint32_t type, unsigned long esr, unsigned long address)
{
	(void) address;

	switch(get_exception_class(esr)){
	case EC_SVC:
	{
		uint32_t sysid = get_syscall_id();
		handle_syscall(type, sysid);
	}
	break;
	default:
		while(1);
	}


	// syscalls will land here
}

void handle_irq(uint32_t type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) address;
	uint32_t irid = gicc_apirq();
	if(esr == 0) {
		switch (irid) {
		case IRID_SYSTIMER:
			gic400_sched_timer();
			uart_print("Timer Interrupt\n");
			/* Reset the timer */
			timer_init();

		case IRID_TIMER_SGI:
			/* Clear the interrupt otherwise it won't be fired again.*/
			gicc_eoi(irid);
			/* Call the scheduler with interrupts enabled 
			 * (disabled in an execption handler) */
			enable_irq();
			scheduler();
			disable_irq();
		break;
		case IRID_GPIO_BANK0:
			/* Clear the irq in the gic. */
			gicc_eoi(IRID_GPIO_BANK0);
			/* Clear the gpio event register, or it will be fired again.*/
			gpio_clear_event(16);
			/* Let the (Watch)Dog out ;)! */
			mmio_write32(PM_WDOG, PM_PASSWORD | 1);
			mmio_write32(PM_RSTC, PM_PASSWORD | PM_RSTC_RESET);
		break;
		default:
			uart_print("Unknown Interrupt\n");
			print_core_id();
			gicc_eoi(irid);
		}
	} else {
		/* O.o Unexpected! */
		uart_print("Unknown Interrupt\n");
		while(1); 
	}
}
