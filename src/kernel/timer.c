#include "peripherals.h"
#include "io.h"
#include "timer.h"

static volatile sys_timer* timer = (sys_timer*)SYS_TIMER_BASE; 

void timer_init(void){
  unsigned int cur = mmio_read(SYS_TIMER_CLO);
  // system timer runs at 1 MHz == 1 sec
  cur += 2 * 5000000;
  
  //timer->c1 = cur;
  //timer->cs = 1;

  mmio_write(SYS_TIMER_C1, cur);
  mmio_write(SYS_TIMER_CS, 0x1 << 1);
}


void delay(unsigned int delay){
  unsigned int start = mmio_read(SYS_TIMER_CLO);

  while(1){
    unsigned int cur = mmio_read(SYS_TIMER_CLO);
    if((cur - start) > delay)
      break;
  }
}
