#ifndef MM_H
#define MM_H

extern unsigned long get_free_page();
extern void free_page(unsigned long p);
extern void memzero(void* src, unsigned long n);
extern uint64_t v2pa(uint64_t vaddr);
extern uint64_t p2va(uint64_t paddr);

#endif

