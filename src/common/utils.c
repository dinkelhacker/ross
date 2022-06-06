#include <stdint.h>
#include "asm_utils.h"
#include "io.h"

void print_current_el(void){
	uint32_t exceptionLevel = get_el();
	if(exceptionLevel == 1)
		uart_print("Running at EL1 \n");
	else if(exceptionLevel == 2)
		uart_print("Running at EL2 \n");
	else if(exceptionLevel == 3)
		uart_print("Running at EL3 \n");
}

void print_core_id(void){
	uint32_t core = get_core_id();
	if(core == 0)
		uart_print("Running on Core 0\n");
	else if(core == 1)
		uart_print("Running on Core 1\n");
	else if(core == 2)
		uart_print("Running on Core 2\n");
	else if(core == 3)
		uart_print("Running on Core 3\n");
}

inline __attribute__((always_inline)) void ensure_coheret_execution(uint64_t addr)
{
		__asm__ volatile (
		"dc	cvau, %0\n"
		"dsb	ish\n"
		"ic	ivau, %0\n"
		"dsb	ish\n"
		"isb"
		:: "r" (addr));
}

void relocate_code(uint64_t *start, uint64_t *end, uint64_t * dest)
{
	while (start < end) {
		*dest = *start;
		ensure_coheret_execution(dest);
		start++;
		dest++;

	}
}
