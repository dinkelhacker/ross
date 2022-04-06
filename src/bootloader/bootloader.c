#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "kernel_loader.h"
#include "memory_layout.h"
#include "asm_utils.h"
#include "utils.h"
 
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
	uint32_t core = get_core_id() - 1;
	cpu_boot_status *status = BOOT_CORE_STATUS;
	while((status + core)->core_released != CORE_RELEASED)
	{
		__asm__("wfe");
	};
	uart_writeText("Released!");
	print_core_id();
	jumpToAddr((status + core)->addr);
}
