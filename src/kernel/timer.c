#include "peripherals.h"
#include "io.h"
#include "timer.h"
#include <stdint.h>

static volatile sys_timer* timer = (sys_timer*) SYS_TIMER_BASE; 

void timer_init(void)
{
	uint32_t cur = timer->clo;
	// system timer runs at 1 MHz == 1 sec
	cur += 2 * 5000000;
	
	timer->c1 = cur;
	timer->cs = (uint32_t) (0x1 << 1);
}


void delay(uint32_t delay)
{
	uint32_t start = timer->clo;

	while(1){
		uint32_t cur = timer->clo;
		if((cur - start) > delay) break;
	}
}
