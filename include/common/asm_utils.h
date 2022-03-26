#ifndef ASM_UTILS_H
#define ASM_UTILS_H
#include <stdint.h>

extern void jumpToAddr(void*);
extern uint32_t getExceptionLevel();
#endif
