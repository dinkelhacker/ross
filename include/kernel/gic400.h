#ifndef __GIC_400_H__
#define __GIC_400_H__

#include "types.h"

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

typedef struct {
	volatile uint ctl;
	volatile const uint type;
	volatile const uint iid;
	volatile uint _res0[((0x80 - 0xC) / (sizeof(uint)))];
	volatile uint igroup[((0x100 - 0x80) / (sizeof(uint)))];
	volatile uint isenable[((0x180 - 0x100) / (sizeof(uint)))];
	volatile uint icenable[((0x200 - 0x180) / (sizeof(uint)))];
	volatile uint ispend[((0x280 - 0x200) / (sizeof(uint)))];
	volatile uint icpend[((0x300 - 0x280) / (sizeof(uint)))];
	volatile uint isactive[((0x380 - 0x300) / (sizeof(uint)))];
	volatile uint icactive[((0x400 - 0x380) / (sizeof(uint)))];
	volatile uint ipriority[((0x800 - 0x400) / (sizeof(uint)))];
	volatile uint istargets[((0xC00 - 0x800) / (sizeof(uint)))];
	volatile uint icfg[((0xD00 - 0xC08) / (sizeof(uint)))];
	volatile const uint ppis;
	volatile uint spis[((0xF00 - 0xD04) / (sizeof(uint)))];
	volatile uint sgi;
	volatile uint _res1[((0xF10 - 0xF04) / (sizeof(uint)))];
	volatile uint cpendsgi[((0xF20 - 0xF10) / (sizeof(uint)))];
	volatile uint spendsgi[((0xF30 - 0xF20) / (sizeof(uint)))];
	volatile uint _res2[((0xFD0 - 0xF30) / (sizeof(uint)))];
	volatile const uint pid[((0xFF0 - 0xFD0) / (sizeof(uint)))];
	volatile const uint cid[((0x1000 - 0xFF0) / (sizeof(uint)))];
} gic400_gicd_t;

typedef struct {
	volatile uint ctl;
	volatile uint pm;
	volatile uint bp;
	volatile const uint ia;
	volatile uint eoi;
	volatile const uint rp;
	volatile const uint hppi;
	volatile uint abp;
	volatile const uint aia;
	volatile uint aeoi;
	volatile const uint ahppi;
	volatile uint _res0[(0xD0 - 0x2C) / sizeof(uint)];
	volatile uint ap;
	volatile uint _res1[(0xE0 - 0xD4) / sizeof(uint)];
	volatile uint nasp;
	volatile uint _res2[(0xFC - 0xE4) / sizeof(uint)];
	volatile const uint iid;
	volatile uint _res3[(0x1000 - 0x100) / sizeof(uint)];
	volatile uint dir;
} gic400_gicc_t;

extern int gic400_init(void* interrupt_controller_base);
extern uint gicc_apirq();
extern void gicc_eoi(uint irid);
#endif

