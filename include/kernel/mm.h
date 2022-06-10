#ifndef MM_H
#define MM_H

extern uint64_t get_free_kpage();
extern uint64_t get_free_virt_kpage();
extern uint64_t get_free_upage();
extern uint64_t get_free_virt_upage();
extern void free_page(unsigned long p);
extern void memzero(void* src, unsigned long n);
extern uint64_t v2pa(uint64_t vaddr);
extern uint64_t p2va(uint64_t paddr);

struct mmap {
	uint64_t *table;
};

//struct page {
//
//}

#endif

