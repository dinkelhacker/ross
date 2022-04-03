#ifndef ASM_UTILS_H
#define ASM_UTILS_H
#include <stdint.h>

extern void jumpToAddr(void*);
extern uint32_t getExceptionLevel(void);
extern uint32_t get_core_id(void);
extern void wakeup_cores(void);
#endif
