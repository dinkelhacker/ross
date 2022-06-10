#include "memory_layout.h"
#include <stdint.h>
static unsigned short kmem_map [PAGES_KERNEL] = {0,};
static unsigned short umem_map [PAGES_USER] = {0,};

uint64_t v2pa(uint64_t vaddr)
{
	return vaddr - KERNEL_VA_OFFSET;
}

uint64_t p2va(uint64_t paddr)
{
	if (paddr >= MEMORY_BOTTOM_USER && paddr < MEMORY_TOP_USER)
		return paddr - USER_VA_OFFSET;
	else
		return paddr + KERNEL_VA_OFFSET;
}

uint64_t get_free_kpage()
{
	for (uint32_t i = 0; i < PAGES_KERNEL; i++) {
		if (kmem_map[i] == 0){
			kmem_map[i] = 1;
			return MEMORY_BOTTOM_KERNEL + i*PAGE_SIZE;
		}
	}
	return 0;
}

uint64_t get_free_virt_kpage()
{
	uint64_t page = get_free_kpage();
	if (page == 0)
		return -1; // TODO
	else
		return p2va(page);
}

uint64_t get_free_upage()
{
	for (uint32_t i = 0; i < PAGES_USER; i++) {
		if (umem_map[i] == 0){
			umem_map[i] = 1;
			return MEMORY_BOTTOM_USER_PAGING + i*PAGE_SIZE;
		}
	}
	return 0;
}

uint64_t get_free_virt_upage()
{
	uint64_t page = get_free_upage();
	if (page == 0)
		return -1;
	else
		return p2va(page);
}

void free_kpage(unsigned long p)
{
	kmem_map[(v2pa(p) - MEMORY_BOTTOM_KERNEL) / PAGE_SIZE] = 0;
}
