#ifndef TIMER_H
#define TIMER_H

#include "types.h"

extern void timer_init(void);
extern void delay(uint delay);

typedef struct {
	volatile uint cs; 
	volatile uint clo;
	volatile uint chi;
	volatile uint c0;
	volatile uint c1; 
	volatile uint c2;
	volatile uint c3; 
} sys_timer;

#endif
