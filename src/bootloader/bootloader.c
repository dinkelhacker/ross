#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "memory_layout.h"
#include "asm_utils.h"
#include "utils.h"
 
static void load_kernel(void* dst_addr);
static void copyKernelToAddr(uint32_t numBytes, char * dst_addr);
extern void wait_for_release(cpu_boot_status *core, uint32_t released_flag);

void bootloader_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
	(void) dtb_ptr32;
	(void) x1;
	(void) x2;
	(void) x3;

	uart_init();
	uart_print("Bootloader running... \n");
	uart_print("Waiting to receive Kernel via UART\n");
	load_kernel((void*) 0x160000);

	while (1){
		// echo
		uart_writeByteBlocking(uart_readByteBlocking()); 
	}
}

static void load_kernel(void* dst_addr)
{
	uint32_t kernel_size;
	// wait for boot commadn
	if(readString("boot?")) {
		// confirm command
		uart_print("Ok\n");
		// read kernel size
		kernel_size = uart_readIntBlocking();
		// confirm kernel size
		uart_sendInt(kernel_size);
		//read and copy kernel
		copyKernelToAddr(kernel_size, dst_addr);
		// jump to kernel
		jump_to(dst_addr);
	}
}

static void copyKernelToAddr(uint32_t numBytes, char * dst_addr)
{
	for(uint32_t i = 0; i < numBytes; i++) {
		char c = uart_readByteBlocking();
		*dst_addr = c;
		dst_addr++;
	}
}

void waiting_core()
{
	uint32_t core = get_core_id() - 1;
	cpu_boot_status *status = BOOT_CORE_STATUS;
	status += core;
	wait_for_release(status, CORE_RELEASED);
}
