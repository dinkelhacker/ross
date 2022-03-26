#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

extern void timer_init(void);
extern void delay(uint32_t delay);

typedef struct {
	volatile uint32_t cs; 
	volatile uint32_t clo;
	volatile uint32_t chi;
	volatile uint32_t c0;
	volatile uint32_t c1; 
	volatile uint32_t c2;
	volatile uint32_t c3; 
} sys_timer;

#endif
