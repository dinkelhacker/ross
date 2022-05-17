#ifndef MMU_ARMV8A_H
#define MMU_ARMV8A_H

extern void mmu_init(void);
extern void mmu_setup_tables();
extern void mmu_enable();
extern void mmu_flush_tlb_e3();

#endif
