#include "peripherals.h"
#include "io.h"
#include "timer.h"
#include "types.h"

static volatile sys_timer* timer = (sys_timer*)SYS_TIMER_BASE; 

void timer_init(void)
{
	uint cur = mmio_read(SYS_TIMER_CLO);
	// system timer runs at 1 MHz == 1 sec
	cur += 2 * 5000000;
	

	mmio_write(SYS_TIMER_C1, cur);
	mmio_write(SYS_TIMER_CS, 0x1 << 1);
}


void delay(uint delay)
{
	uint start = mmio_read(SYS_TIMER_CLO);

	while(1){
		uint cur = mmio_read(SYS_TIMER_CLO);
		if((cur - start) > delay) break;
	}
}
