#include <stdint.h>
#include "asm_utils.h"
#include "io.h"

void print_current_el(void){
	uint32_t exceptionLevel = getExceptionLevel();
	if(exceptionLevel == 1)
		uart_writeText("Running at EL1 \n");
	else if(exceptionLevel == 2)
		uart_writeText("Running at EL2 \n");
	else if(exceptionLevel == 3)
		uart_writeText("Running at EL3 \n");
}

void print_core_id(void){
	uint32_t core = get_core_id();
	if(core == 0)
		uart_writeText("Running on Core 0\n");
	else if(core == 1)
		uart_writeText("Running on Core 1\n");
	else if(core == 2)
		uart_writeText("Running on Core 2\n");
	else if(core == 3)
		uart_writeText("Running on Core 3\n");
}
