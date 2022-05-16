#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "memory_layout.h"
#include "asm_utils.h"
#include "utils.h"
 
static void load_kernel(void* dst_addr);
static void cpy2addr(uint32_t numBytes, char * dst_addr);
extern void wait_for_release(volatile cpu_boot_status *core, uint32_t released_flag);
uint64_t lvl0_table[512] __attribute__((aligned(4096)));
uint64_t lvl1_table[4] __attribute__((aligned(4096)));

static void _set_tcr_el3(uint64_t tcr_el3)
{
	__asm__ volatile ("msr	tcr_el3, %0" :: "r" (tcr_el3));
}

static void _set_ttbr0_el3(uint64_t * ttrbr0)
{
	__asm__ volatile ("msr	ttbr0_el3, %0" :: "r" (ttrbr0));
}

static void _set_mair_el3(uint64_t mair_el1)
{
	__asm__ volatile ("msr	mair_el3, %0" :: "r" (mair_el1));
}

void bootloader_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
{
	(void) dtb_ptr32;
	(void) x1;
	(void) x2;
	(void) x3;

	uart_init();
	uart_do_print("Bootloader running... \n");
	uart_do_print("Waiting to receive Kernel via UART\n");
	load_kernel((void*) 0x160000);

	while (1){
		// echo
		uart_writeByteBlocking(uart_read_byte_blocking()); 
	}
}

#define MAIR_EL3 0x000000000000FF44

#define TT_S1_DEVICE_nGnRnE	(uint64_t) 0x00600000000000409    /* Index = 2, AF=1, PXN=1, UXN=1 */
#define TT_S1_NORMAL_NO_CACHE 	(uint64_t) 0x00000000000000401    /* Index = 0, AF=1 */
#define TT_S1_NORMAL_WBWA     	(uint64_t) 0x00000000000000405    /* Index = 1, AF=1 */
#define TT_S1_TABLE 		(uint64_t) 0x00000000000000003 	  /* NSTable=0, PXNTable=0, UXNTable=0, APTable=0 */
#define TT_S1_PAGE		(uint64_t) 0x3
#define TT_S1_OUTER_SHARED 	(uint64_t) (2 << 8) // Outer-shareable
#define TT_S1_INNER_SHARED 	(uint64_t) (3 << 8) // Inner-shareable


static void load_kernel(void* dst_addr)
{
	uint32_t kernel_size;
	// wait for boot commadn
	if(readString("boot?")) {
		// confirm command
		uart_do_print("Ok\n");
		// read kernel size
		kernel_size = uart_read_u32();
		// confirm kernel size
		uart_sendInt(kernel_size);
		//read and copy kernel
		cpy2addr(kernel_size, dst_addr);
		// jump to kernel

		lvl0_table[0] = TT_S1_TABLE |
		(uint64_t) lvl1_table;
//		// [0] 0x0000 0000 - 40 3FFF FFFF
//		lvl1_table[0] = TT_S1_NORMAL_WBWA |
//				TT_S1_INNER_SHARED |
//				(uint64_t) 0x0;
//		
//		/* [1]: 0x4000,0000 - 0x40 7FFF,FFFF */
//		lvl1_table[1] = TT_S1_NORMAL_WBWA |
//				TT_S1_INNER_SHARED |
//				(uint64_t) 0x40000000;
//
//		/* [2]: 0x8000,0000 - 0x40 BFFF,FFFF */
//		lvl1_table[2] = TT_S1_NORMAL_WBWA |
//				TT_S1_INNER_SHARED |
//				(uint64_t) 0x80000000;
//
//		/* [3]: 0xC000,0000 - 0x40 FFFF,FFFF */
//		lvl1_table[3] = TT_S1_DEVICE_nGnRnE |
//			TT_S1_INNER_SHARED |
//			(uint64_t) 0xC0000000;

		/* Level 0 table */
		/* [0]: 40 0000 0000 - ... */
		
		lvl0_table[0] = TT_S1_TABLE |
		(uint64_t) lvl1_table;
		// [0] 40 0000 0000 - 40 3FFF FFFF
		lvl1_table[0] = TT_S1_NORMAL_NO_CACHE |
				TT_S1_INNER_SHARED |
				(uint64_t) 0x0;
		
		/* [1]: 0x40 4000,0000 - 0x40 7FFF,FFFF */
		lvl1_table[1] = TT_S1_NORMAL_NO_CACHE |
				TT_S1_INNER_SHARED |
				(uint64_t) 0x40000000;

		/* [2]: 0x40 8000,0000 - 0x40 BFFF,FFFF */
		lvl1_table[2] = TT_S1_NORMAL_NO_CACHE |
				TT_S1_INNER_SHARED |
				(uint64_t) 0x80000000;

		/* [3]: 0x40 C000,0000 - 0x40 FFFF,FFFF */
		lvl1_table[3] = TT_S1_DEVICE_nGnRnE |
			TT_S1_INNER_SHARED |
			(uint64_t) 0xC0000000;

		lvl1_table[256] = TT_S1_NORMAL_NO_CACHE |
				TT_S1_INNER_SHARED |
				(uint64_t) 0x0;
		
		/* [1]: 0x40 4000,0000 - 0x40 7FFF,FFFF */
		lvl1_table[257] = TT_S1_NORMAL_NO_CACHE |
				TT_S1_INNER_SHARED |
				(uint64_t) 0x40000000;

		/* [2]: 0x40 8000,0000 - 0x40 BFFF,FFFF */
		lvl1_table[258] = TT_S1_NORMAL_NO_CACHE |
				TT_S1_INNER_SHARED |
				(uint64_t) 0x80000000;

		/* [3]: 0x40 C000,0000 - 0x40 FFFF,FFFF */
		lvl1_table[259] = TT_S1_DEVICE_nGnRnE |
			TT_S1_INNER_SHARED |
			(uint64_t) 0xC0000000;


		/* set translation table base register 0 (el 1). */
		_set_ttbr0_el3(lvl0_table);
		/* set memory attribute indirection register (el 1). */
		_set_mair_el3(MAIR_EL3);
		/* set translation control register (el 1). */
		_set_tcr_el3(
		(16)	  | /* t0sz = va space is 48 bits. */
		(0x1<<8)  | /* irgn0 = 0b01. walks to ttbr0 are inner wb/wa. */
		(0x1<<10) | /* orgn0 = 0b01. walks to ttbr0 are outer wb/wa. */
		(0x3<<12) | /* sh0 = 0b11. inner sharable. */
			    /* tbi0 = 0b0. top byte not ignored. */
			    /* tg0 = 0b00. 4kb granule. */
			    /* ips = 0. 32-bit pa space. */
		(0x5 << 16)
		);

		jump_to(dst_addr);
	}
}

static void cpy2addr(uint32_t numBytes, char * dst_addr)
{
	for(uint32_t i = 0; i < numBytes; i++) {
		char c = uart_read_byte_blocking();
		*dst_addr = c;
		dst_addr++;
	}
}

void waiting_core()
{
	/* Translation table was already set up. */

	/* set translation table base register 0 (el 3). */
	_set_ttbr0_el3(lvl0_table);
	/* set memory attribute indirection register (el 3). */
	_set_mair_el3(MAIR_EL3);
	/* set translation control register (el 3). */
	_set_tcr_el3(
	(16)	  | /* t0sz = va space is 48 bits. */
	(0x1<<8)  | /* irgn0 = 0b01. walks to ttbr0 are inner wb/wa. */
	(0x1<<10) | /* orgn0 = 0b01. walks to ttbr0 are outer wb/wa. */
	(0x3<<12) | /* sh0 = 0b11. inner sharable. */
		    /* tbi0 = 0b0. top byte not ignored. */
		    /* tg0 = 0b00. 4kb granule. */
		    /* ips = 0. 32-bit pa space. */
	(0x5 << 16)
	);

	uint32_t core = get_core_id() - 1;
	volatile cpu_boot_status *status = (cpu_boot_status *) BOOT_CORE_STATUS;
	wait_for_release(&status[core], CORE_RELEASED);
}
