#ifndef ASM_UTILS_H
#define ASM_UTILS_H
#include <stdint.h>

extern void jump_to(void*);
extern uint32_t get_el(void);
extern uint32_t get_core_id(void);
#endif
