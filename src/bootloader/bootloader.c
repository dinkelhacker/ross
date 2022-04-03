#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "kernel_loader.h"
#include "memory_layout.h"
 
void bootloader_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
	(void) dtb_ptr32;
	(void) x1;
	(void) x2;
	(void) x3;

	uart_init();
	uart_writeText("Bootloader running... \n");
	uart_writeText("Waiting to receive Kernel via UART\n");
	load_kernel((void*) 0x160000);

	while (1){
		// echo
		uart_writeByteBlocking(uart_readByteBlocking()); 
	}
}


void waiting_core()
{
	volatile uint32_t *p =  CORE_RELEASE;
	while(*p != 0xabcdef){};
	uart_writeText("Released!");
}
