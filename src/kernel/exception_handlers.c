#include "exception_types.h"
#include "io.h"
#include "peripherals.h"
#include "irids.h"
#include "gic400.h"
#include "timer.h"
#include "tasks.h"
#include "scheduler.h"
#include "exceptions.h"
#include "syscall.h"

void
invalid_exception(uint32_t type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void
handle_sync(unsigned long esr, unsigned long address, uint32_t sysid)
{
	(void) esr;
	(void) address;
	(void) sysid;
	//TODO: 
	//	- check esr if caused by SVC
	//	- maybe just grab the esr from here instead of passing it
	switch (sysid){
	case SYSC_TASK_SUSPEND:
		enable_irq();
		uart_writeText("Syscall\n");
		scheduler();
		disable_irq();
	}

	// syscalls will land here
}

void
handle_irq(uint32_t type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;

	uint32_t irid = gicc_apirq();
	if(esr == 0) {
		switch (irid) {
		case IRID_SYSTIMER:
			uart_writeText("Timer Interrupt\n");
			// reset the timer
			timer_init();
			// clear the interrupt otherwise it won't be fired again
			gicc_eoi(IRID_SYSTIMER);

			//call the scheduler with interrupts enabled (disabled in an execption handler)
			enable_irq();
			scheduler();
			disable_irq();
		break;
		default:
			uart_writeText("Unknown Interrupt\n");
		}
	} else {
		/* O.o Unexpected! */
		uart_writeText("Unknown Interrupt\n");
		while(1); 
	}
}
