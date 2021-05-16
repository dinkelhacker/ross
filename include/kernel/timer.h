#ifndef TIMER_H
#define TIMER_H

extern void timer_init(void);
extern void delay(unsigned int delay);

typedef struct {
  volatile unsigned int cs; 
  volatile unsigned int clo;
  volatile unsigned int chi;
  volatile unsigned int c0;
  volatile unsigned int c1; 
  volatile unsigned int c2;
  volatile unsigned int c3; 
} sys_timer;

#endif
