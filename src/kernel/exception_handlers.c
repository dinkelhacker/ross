#include "exception_types.h"
#include "io.h"
#include "peripherals.h"
#include "irids.h"
#include "gic400.h"
#include "timer.h"
#include "tasks.h"
#include "scheduler.h"
#include "exceptions.h"

// EL1t
void handle_sync_el1t(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_irq_el1t(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_fiq_el1t(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_serror_el1t(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}


// EL1h
void handle_sync_el1h(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_irq_el1h(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;

	unsigned int irid = gicc_apirq();
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
}

void handle_fiq_el1h(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_serror_el1h(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

// EL0_64
void handle_sync_el0_64(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_irq_el0_64(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_fiq_el0_64(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_serror_el0_64(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

/* EL0_32 */
void handle_sync_el0_32(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_irq_el0_32(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_fiq_el0_32(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}

void handle_serror_el0_32(unsigned int type, unsigned long esr, unsigned long address)
{
	(void) type;
	(void) esr;
	(void) address;
	while(1);
}


