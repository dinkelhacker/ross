#ifndef ASM_H
#define ASM_H
#include "utils.h"

extern void setup_core(volatile cpu_boot_status *core, uint32_t addr, uint32_t release_flag);
extern void wakeup_cores(void);
#endif
