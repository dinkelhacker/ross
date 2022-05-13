#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H

#ifndef QEMU_DEBUG
#define KERNEL_BASE       0x160000
//#define KERNEL_BASE       0x8000000000000000
#else
#define KERNEL_BASE       0x80000
#endif
#define KERNEL_STACK_CORE0	KERNEL_BASE - 0x10000
#define KERNEL_STACK_CORE1	KERNEL_BASE - 0x20000
#define KERNEL_STACK_CORE2	KERNEL_BASE - 0x30000
#define KERNEL_STACK_CORE3	KERNEL_BASE - 0x40000

#define KCODE_SIZE		0x100000
#define KDATA_SIZE		0x100000
#define KD_SIZE		0x100000

#define BOOTLOADER_BASE		0x80000
#define BOOT_CORE_STATUS	0x40000

#define MEMORY_TOP        0xFE000000
#define MEMORY_BOTTOM     4 * SECTION_SIZE

#define PAGE_SHIFT        12
#define TABLE_SHIFT       9
#define SECTION_SHIFT     (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE         (1 << PAGE_SHIFT)            // 4096
#define SECTION_SIZE      (1 << SECTION_SHIFT)         // 2 ^ 21 (1MB)

#define PAGING_MEM        (MEMORY_TOP - MEMORY_BOTTOM) // start of paging memory
#define PAGES             (PAGING_MEM/PAGE_SIZE)       // number of pages

#endif
