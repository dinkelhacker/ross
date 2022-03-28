#include <stdint.h>
#include "asm_utils.h"
#include "io.h"

void 
print_current_el(void){
	uint32_t exceptionLevel = getExceptionLevel();
	if(exceptionLevel == 1)
		uart_writeText("Running at EL1 \n");
	else if(exceptionLevel == 2)
		uart_writeText("Running at EL2 \n");
	else if(exceptionLevel == 3)
		uart_writeText("Running at EL3 \n");
	
}
