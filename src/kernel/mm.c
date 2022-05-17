#include "memory_layout.h"
#include <stdint.h>
static unsigned short mem_map [PAGES] = {0,};

uint64_t v2pa(uint64_t vaddr)
{
	return vaddr - KERNEL_VA_OFFSET;
}

uint64_t p2va(uint64_t paddr)
{
	return paddr + KERNEL_VA_OFFSET;
}

unsigned long get_free_page()
{
	for (uint32_t i = 0; i < PAGES; i++) {
		if (mem_map[i] == 0){
			mem_map[i] = 1;
			return p2va(MEMORY_BOTTOM + i*PAGE_SIZE);
		}
	}
	return 0;
}

void free_page(unsigned long p)
{
	mem_map[(v2pa(p) - MEMORY_BOTTOM) / PAGE_SIZE] = 0;
}
