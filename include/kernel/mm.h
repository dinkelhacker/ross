#ifndef MM_H
#define MM_H

extern unsigned long get_free_page();
extern void free_page(unsigned long p);
extern void memzero(void* src, unsigned long n);

#endif

