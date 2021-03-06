#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H

#ifndef QEMU_DEBUG
#define KERNEL_LOAD_OFFSET		0x160000
#else
#define KERNEL_LOAD_OFFSET      	0x80000
#endif

#define KERNEL_VA_OFFSET		0x4000000000
#define USER_VA_OFFSET			0x80000000
#define KERNEL_BASE			KERNEL_VA_OFFSET + KERNEL_LOAD_OFFSET


#define KERNEL_STACK_CORE0		KERNEL_BASE - 0x10000
#define KERNEL_STACK_CORE1		KERNEL_BASE - 0x20000
#define KERNEL_STACK_CORE2		KERNEL_BASE - 0x30000
#define KERNEL_STACK_CORE3		KERNEL_BASE - 0x40000

#define BOOTLOADER_BASE			0x80000
#define BOOT_CORE_STATUS		0x40000

#define MEMORY_TOP_USER			0xC0000000
#define MEMORY_BOTTOM_USER      	0x80000000

#define MEMORY_TOP_USER_PAGING		MEMORY_TOP_USER
#define MEMORY_BOTTOM_USER_PAGING	MEMORY_BOTTOM_USER

#define MEMORY_TOP_KERNEL       	0x80000000
#define MEMORY_BOTTOM_KERNEL    	4 * SECTION_SIZE

#define PAGE_SHIFT        		12
#define TABLE_SHIFT       		9
#define SECTION_SHIFT     		(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE         		(1 << PAGE_SHIFT)            // 4096
#define SECTION_SIZE      		(1 << SECTION_SHIFT)         // 2 ^ 21 (1MB)

// start of paging memory
#define PAGING_MEM_KERNEL       	(MEMORY_TOP_KERNEL - MEMORY_BOTTOM_KERNEL)
#define PAGING_MEM_USER         	(MEMORY_TOP_USER_PAGING - MEMORY_BOTTOM_USER_PAGING)
// number of pages
#define PAGES_KERNEL            	(PAGING_MEM_KERNEL/PAGE_SIZE)
#define PAGES_USER              	(PAGING_MEM_USER/PAGE_SIZE)

#endif
