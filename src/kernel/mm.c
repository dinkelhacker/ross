#include "memory_layout.h"
#include "types.h"
static unsigned short mem_map [PAGES] = {0,};

ulong get_free_page()
{
	for (int i = 0; i < PAGES; i++) {
		if (mem_map[i] == 0){
			mem_map[i] = 1;
			return MEMORY_BOTTOM + i*PAGE_SIZE;
		}
	}
	return 0;
}

void free_page(ulong p){
	mem_map[(p - MEMORY_BOTTOM) / PAGE_SIZE] = 0;
}
