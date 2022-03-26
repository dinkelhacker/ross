#include "io.h"
#include <stdbool.h>
#include "asm_utils.h"
#include "utils.h"
#include <stdint.h>

static void copyKernelToAddr(uint32_t numBytes, char * dst_addr);

void load_kernel(void* dst_addr)
{
	uint32_t kernel_size;
	// wait for boot commadn
	if(readString("boot?")) {
		// confirm command
		uart_writeText("Ok\n");
		// read kernel size
		kernel_size = uart_readIntBlocking();
		// confirm kernel size
		uart_sendInt(kernel_size);
		//read and copy kernel
		copyKernelToAddr(kernel_size, dst_addr);
		// jump to kernel
		jumpToAddr(dst_addr);
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
