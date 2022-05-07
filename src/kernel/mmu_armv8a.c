#include <stdint.h>
#include "mm.h"
#include "memory_layout.h"

static uint64_t *lvl1_table;
static uint64_t *lvl2_table;
static uint64_t *lvl3_table;

/* Set up memory attributes
 * 
 * This equates to:
 * 0 = b01000100 = Normal, Inner/Outer Non-Cacheable
 * 1 = b11111111 = Normal, Inner/Outer WB/WA/RA
 * 2 = b00000000 = Device-nGnRnE
*/
#define MAIR_EL1 0x000000000000FF44

#define TT_S1_DEVICE_nGnRnE	(uint64_t) 0x00600000000000409    /* Index = 2, AF=1, PXN=1, UXN=1 */
#define TT_S1_NORMAL_NO_CACHE 	(uint64_t) 0x00000000000000401    /* Index = 0, AF=1 */
#define TT_S1_NORMAL_WBWA     	(uint64_t) 0x00000000000000405    /* Index = 1, AF=1 */
#define TT_S1_TABLE 		(uint64_t) 0x00000000000000003 	  /* NSTable=0, PXNTable=0, UXNTable=0, APTable=0 */
#define TT_S1_PAGE		(uint64_t) 0x3
#define TT_S1_OUTER_SHARED 	(uint64_t) (2 << 8) // Outer-shareable
#define TT_S1_INNER_SHARED 	(uint64_t) (3 << 8) // Inner-shareable

static void _set_ttbr0_el1(uint64_t * ttrbr0);
static void _set_mair_el1(uint64_t mair_el1);
static void _set_tcr_el1(uint64_t tcr_el1);
static void _enable_mmu(void);

void mmu_setup_tables()
{
	/* Each page is 4k.
	 * Translation tables have to be 4k aligned.
	 * Each table is 512 elements @ uint64_t. */
	lvl1_table = (uint64_t *) get_free_page();
	lvl2_table = (uint64_t *) get_free_page();
	lvl3_table = (uint64_t *) get_free_page();
	
	memzero((void*) lvl1_table, PAGE_SIZE);
	memzero((void*) lvl2_table, PAGE_SIZE);
	memzero((void*) lvl3_table, PAGE_SIZE);

	/* Level 1 table */
	/* [0]: 0x0000,0000 - 0x3FFF,FFFF */ 
	lvl1_table[0] = TT_S1_TABLE |
			(uint64_t) lvl2_table;
	
	/* [1]: 0x4000,0000 - 0x7FFF,FFFF */
	lvl1_table[1] = TT_S1_NORMAL_WBWA |
			TT_S1_INNER_SHARED |
			(uint64_t) 0x40000000;

	/* [2]: 0x8000,0000 - 0xBFFF,FFFF */
	lvl1_table[2] = TT_S1_NORMAL_WBWA |
			TT_S1_INNER_SHARED |
			(uint64_t) 0x80000000;

	/* [3]: 0xC000,0000 - 0xFFFF,FFFF */
	lvl1_table[3] = TT_S1_DEVICE_nGnRnE |
			TT_S1_INNER_SHARED |
			(uint64_t) 0xC0000000;


	/* Level 2 table */
	/* [0]: 0x0000,0000 - 0x01FF,FFF */
	lvl2_table[0] = TT_S1_TABLE |
			(uint64_t) lvl3_table;
	
	/* ... until 0x3FFF,FFFF */
	uint64_t addr = 0x200000;
	for (uint32_t i= 1; i < 512; i++) {
		
		lvl2_table[i] = TT_S1_NORMAL_WBWA |
				TT_S1_INNER_SHARED |
				(i * addr);
	}


	/* Level 3 table */
	addr = 0x1000;
	/* [0..63]: 0x0000,0000 - 0x0003,FFFF */
	for (uint32_t i = 0; i < 64; i++) {
		lvl3_table[i] = TT_S1_NORMAL_WBWA | 
				TT_S1_INNER_SHARED |
				TT_S1_PAGE |
				(i * addr);
	}
	
	/* [64 - 79] 0x0004,0000 - 0x0004,FFFF */
	for (uint32_t i = 64; i < 80; i++) {
		lvl3_table[i] = TT_S1_NORMAL_NO_CACHE| 
				TT_S1_INNER_SHARED |
				TT_S1_PAGE |
				(i * addr);
	}

	/* [80..511]: 0x0005,0000 - 0x01FF,FFFF */
	for (uint32_t i = 80; i < 512; i++) {
		lvl3_table[i] = TT_S1_NORMAL_WBWA | 
				TT_S1_INNER_SHARED |
				TT_S1_PAGE |
				(i * addr);
	}
}

void mmu_init()
{
	/* Set translation table base register 0 (EL 1). */
	_set_ttbr0_el1(lvl1_table);
	/* Set memory attribute indirection register (EL 1). */
	_set_mair_el1(MAIR_EL1);
	/* Set translation control register (EL 1). */
	_set_tcr_el1(
		(32)	  | /* T0SZ = VA space is 32 bits. */
		(0x1<<8)  | /* IRGN0 = 0b01. Walks to TTBR0 are Inner WB/WA. */
		(0x1<<10) | /* ORGN0 = 0b01. Walks to TTBR0 are Outer WB/WA. */
		(0x3<<12) | /* SH0 = 0b11. Inner Sharable. */
		(0x1<<23)   /* EPD1 = 0b01. Disable table walks from TTBR1 */
			    /* TBI0 = 0b0. Top byte not ignored. */
			    /* TG0 = 0b00. 4KB granule. */
			    /* IPS = 0. 32-bit PA space. */
		);
	_enable_mmu();

}


static void _enable_mmu()
{
	__asm__ volatile (
		"dsb sy\n"
		"mov 	x0, #(1 << 0)\n"   	// M=1 Enable the stage 1 MMU
		"orr	x0, x0, #(1 << 2)\n" 	// C=1 Enable data and unified caches
		"orr	x0, x0, #(1 << 12)\n"   // I=1 Enable instruction fetches to allocate into unified caches
		"msr	sctlr_el1, x0\n"
		"isb" ::);
}

static void _set_tcr_el1(uint64_t tcr_el1)
{
	__asm__ volatile ("msr	tcr_el1, %0" :: "r" (tcr_el1));
}

static void _set_ttbr0_el1(uint64_t * ttrbr0)
{
	__asm__ volatile ("msr	ttbr0_el1, %0" :: "r" (ttrbr0));
}

static void _set_mair_el1(uint64_t mair_el1)
{
	__asm__ volatile ("msr	mair_el1, %0" :: "r" (mair_el1));
}
