#ifndef __GIC_400_H__
#define __GIC_400_H__

#include <stdint.h>
/* CoreLink GIC-400 Generic Interrupt Controller */

/* Register bit definitions */
#define GIC400_CTL_ENABLE      ( 1 << 0 )
#define GIC400_CTL_DISABLE     ( 0 << 0 )
#define GIC400_CTL_ENABLE_GRP1 ( 1 << 1 )


#define GIC400_TARGET_CPU0 ( 1 << 0 )
#define GIC400_TARGET_CPU1 ( 1 << 1 )
#define GIC400_TARGET_CPU2 ( 1 << 2 )
#define GIC400_TARGET_CPU3 ( 1 << 3 )
#define GIC400_TARGET_CPU4 ( 1 << 4 )
#define GIC400_TARGET_CPU5 ( 1 << 5 )
#define GIC400_TARGET_CPU6 ( 1 << 6 )
#define GIC400_TARGET_CPU7 ( 1 << 7 )

#define GIC400_ICFG_LEVEL_SENSITIVE ( 0 << 1 )
#define GIC400_ICFG_EDGE_TRIGGERED  ( 1 << 1 )

#define GIC400_ENABLE_GRP1 (1 << 1)

#define GIC_GROUP1 (uint32_t) 0x1

typedef struct {
	volatile uint32_t ctl;
	volatile const uint32_t type;
	volatile const uint32_t iid;
	volatile uint32_t _res0[((0x80 - 0xC) / (sizeof(uint32_t)))];
	volatile uint32_t igroup[((0x100 - 0x80) / (sizeof(uint32_t)))];
	volatile uint32_t isenable[((0x180 - 0x100) / (sizeof(uint32_t)))];
	volatile uint32_t icenable[((0x200 - 0x180) / (sizeof(uint32_t)))];
	volatile uint32_t ispend[((0x280 - 0x200) / (sizeof(uint32_t)))];
	volatile uint32_t icpend[((0x300 - 0x280) / (sizeof(uint32_t)))];
	volatile uint32_t isactive[((0x380 - 0x300) / (sizeof(uint32_t)))];
	volatile uint32_t icactive[((0x400 - 0x380) / (sizeof(uint32_t)))];
	volatile uint32_t ipriority[((0x800 - 0x400) / (sizeof(uint32_t)))];
	volatile uint32_t istargets[((0xC00 - 0x800) / (sizeof(uint32_t)))];
	volatile uint32_t icfg[((0xD00 - 0xC00) / (sizeof(uint32_t)))];
	volatile uint32_t _impldefined[(0xE00 - 0xD00) / sizeof(uint32_t)];
	volatile uint32_t nsacr[((0xF00 - 0xE00) / (sizeof(uint32_t)))];
	volatile uint32_t sgi;
	volatile uint32_t _res1[((0xF10 - 0xF04) / (sizeof(uint32_t)))];
	volatile uint32_t cpendsgi[((0xF20 - 0xF10) / (sizeof(uint32_t)))];
	volatile uint32_t spendsgi[((0xF30 - 0xF20) / (sizeof(uint32_t)))];
	volatile uint32_t _res2[((0xFD0 - 0xF30) / (sizeof(uint32_t)))];
	volatile const uint32_t pid[((0xFF0 - 0xFD0) / (sizeof(uint32_t)))];
	volatile const uint32_t cid[((0x1000 - 0xFF0) / (sizeof(uint32_t)))];
} gic400_gicd_t;

typedef struct {
	volatile uint32_t ctl;
	volatile uint32_t pm;
	volatile uint32_t bp;
	volatile const uint32_t ia;
	volatile uint32_t eoi;
	volatile const uint32_t rp;
	volatile const uint32_t hppi;
	volatile uint32_t abp;
	volatile const uint32_t aia;
	volatile uint32_t aeoi;
	volatile const uint32_t ahppi;
	volatile uint32_t _res0[(0xD0 - 0x2C) / sizeof(uint32_t)];
	volatile uint32_t ap;
	volatile uint32_t _res1[(0xE0 - 0xD4) / sizeof(uint32_t)];
	volatile uint32_t nasp;
	volatile uint32_t _res2[(0xFC - 0xE4) / sizeof(uint32_t)];
	volatile const uint32_t iid;
	volatile uint32_t _res3[(0x1000 - 0x100) / sizeof(uint32_t)];
	volatile uint32_t dir;
} gic400_gicc_t;

extern int gic400_init(void* interrupt_controller_base);
extern int gic400_enable_cpuif();
extern uint32_t gicc_apirq();
extern void gicc_eoi(uint32_t irid);

extern void gic400_enir(uint32_t irid, uint32_t group, uint32_t core);
extern void gic400_ensgi(uint32_t irid, uint32_t group);
extern void gicd_enableir(uint32_t irid);
extern int gicd_groupir(uint32_t irid, uint32_t group);
extern int gicd_irtarget(uint32_t irid, uint32_t cpuid);
void gic400_sched_timer();
#endif

