#ifndef MMU_ARMV8A_H
#define MMU_ARMV8A_H

extern void mmu_init(void);
extern void mmu_setup_tables();
extern void mmu_enable();
extern void mmu_flush_tlb_e3();
extern struct mmap* mmu_create_user_mapping(uint64_t *codep, uint64_t *stackp);
extern void mmu_update_user_mapping(struct mmap *mmap);

#endif
