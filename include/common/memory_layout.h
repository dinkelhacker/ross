#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H

#ifndef QEMU_DEBUG
#define KERNEL_BASE       0x160000
#else
#define KERNEL_BASE       0x80000
#endif
#define BOOTLOADER_BASE   0x80000

#define MEMORY_TOP        0xFE000000
#define MEMORY_BOTTOM     2 * SECTION_SIZE

#define PAGE_SHIFT        12
#define TABLE_SHIFT       9
#define SECTION_SHIFT     (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE         (1 << PAGE_SHIFT)            // 4096
#define SECTION_SIZE      (1 << SECTION_SHIFT)         // 2 ^ 21 (1MB)

#define PAGING_MEM        (MEMORY_TOP - MEMORY_BOTTOM) // start of paging memory
#define PAGES             (PAGING_MEM/PAGE_SIZE)       // number of pages

#endif
