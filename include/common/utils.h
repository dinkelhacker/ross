#ifndef UTILS_H
#define UTILS_H
extern void print_current_el(void);
extern void print_core_id(void);


typedef struct cpu_boot_status{
	uint32_t core_released;
	uint32_t addr;
} cpu_boot_status;

#define CORE_RELEASED 0xabcdef

#endif
