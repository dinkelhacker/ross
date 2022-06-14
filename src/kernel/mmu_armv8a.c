#include <stdint.h>
#include "mm.h"
#include "memory_layout.h"
#include "mmu_armv8a.h"


/* Kernel space */
 static uint64_t lvl0_table[512] __attribute__((aligned(4096)));
 static uint64_t lvl1_table[512] __attribute__((aligned(4096)));
 static uint64_t lvl2_table[512] __attribute__((aligned(4096)));
 static uint64_t lvl3_table[512] __attribute__((aligned(4096)));

 /* User space*/
 static uint64_t us_lvl2_table[512] __attribute__((aligned(4096)));

/* Set up memory attributes
 *
 * This equates to:
 * 0 = b01000100 = Normal, Inner/Outer Non-Cacheable
 * 1 = b11111111 = Normal, Inner/Outer WB/WA/RA
 * 2 = b00000000 = Device-nGnRnE
*/
#define MAIR_EL1 0x000000000000FF44

#define TT_S1_DEVICE_nGnRnE	(uint64_t) 0x0060000000000409    /* Index = 2, AF=1, PXN=1, UXN=1 */
#define TT_S1_NORMAL_NO_CACHE 	(uint64_t) 0x0000000000000401    /* Index = 0, AF=1 */
#define TT_S1_NORMAL_WBWA     	(uint64_t) 0x0000000000000405    /* Index = 1, AF=1 */
#define TT_S1_TABLE 		(uint64_t) 0x0000000000000003 	  /* NSTable=0, PXNTable=0, UXNTable=0, APTable=0 */
#define TT_S1_PAGE		(uint64_t) 0x0000000000000003
#define TT_S1_PAGE_PXN		(uint64_t) 0x0020000000000000
#define TT_S1_TABLE_PXN		(uint64_t) 0x0800000000000000
#define TT_S1_PAGE_EL0_RW	(uint64_t) 0x0000000000000040
#define TT_S1_PAGE_EL0_RO	(uint64_t) 0x00000000000000C0
#define TT_S1_PAGE_UXN		(uint64_t) 0x0040000000000000
#define TT_S1_OUTER_SHARED 	(uint64_t) (2 << 8) // Outer-shareable
#define TT_S1_INNER_SHARED 	(uint64_t) (3 << 8) // Inner-shareable

static void _set_ttbr0_el1(uint64_t * ttrbr0);
static void _set_ttbr1_el1(uint64_t * ttrbr1);
static void _set_mair_el1(uint64_t mair_el1);
static void _set_tcr_el1(uint64_t tcr_el1);

#define U64(value) ((uint64_t) value)

void mmu_setup_tables()
{
	/* Each page is 4k.
	 * Translation tables have to be 4k aligned.
	 * Each table is 512 elements @ uint64_t. */
	//lvl1_table = (uint64_t *) get_free_page();
	//lvl2_table = (uint64_t *) get_free_page();
	//lvl3_table = (uint64_t *) get_free_page();
	//
	//memzero((void*) lvl1_table, PAGE_SIZE);
	//memzero((void*) lvl2_table, PAGE_SIZE);
	//memzero((void*) lvl3_table, PAGE_SIZE);
	//

	/* Kernelspace */
	lvl0_table[0] = TT_S1_TABLE |
			v2pa(U64(lvl1_table));

	lvl1_table[256] = TT_S1_TABLE |
			v2pa(U64(lvl2_table));



	/* [257]: Virt: 0x40 4000,0000 - 0x7FFF,FFFF */
	lvl1_table[257] = TT_S1_NORMAL_WBWA |
			TT_S1_INNER_SHARED |
			(uint64_t) 0x40000000;

	/* [259]: Virt: 0x40 C000,0000 - 0xFFFF,FFFF */
	lvl1_table[259] = TT_S1_DEVICE_nGnRnE |
			(uint64_t) 0xC0000000;




	/* Level 2 table */
	/* [0]: Virt: 0x40 0000,0000 - 0x01FF,FFF */
	lvl2_table[0] = TT_S1_TABLE |
			v2pa(U64(lvl3_table));

	/* ... until 0x40 3FFF,FFFF */
	uint64_t addr = 0x200000;
	for (uint32_t i= 1; i < 512; i++) {

		lvl2_table[i] = TT_S1_NORMAL_WBWA |
				TT_S1_INNER_SHARED |
				(i * addr);
	}


	/* Level 3 table */
	addr = 0x1000;
	/* [0..63]: 0x40 0000,0000 - 0x0003,FFFF */
	for (uint32_t i = 0; i < 64; i++) {
		lvl3_table[i] = TT_S1_NORMAL_WBWA |
				TT_S1_INNER_SHARED |
				TT_S1_PAGE |
				(i * addr);
	}

	/* [64 - 79] 0x40 0004,0000 - 0x0004,FFFF */
	for (uint32_t i = 64; i < 80; i++) {
		lvl3_table[i] = TT_S1_NORMAL_NO_CACHE|
				TT_S1_INNER_SHARED |
				TT_S1_PAGE |
				(i * addr);
	}

	/* [80..511]: 0x40 0005,0000 - 0x01FF,FFFF */
	for (uint32_t i = 80; i < 512; i++) {
		lvl3_table[i] = TT_S1_NORMAL_WBWA |
				TT_S1_INNER_SHARED |
				TT_S1_PAGE |
				(i * addr);
	}



	/* Userspace */
	/* Tasks go here ... */
	/* [0]: Virt: 00 0000,0000 - 0x00 3FFF,FFFF */
	lvl1_table[0] = TT_S1_TABLE |
			TT_S1_TABLE_PXN |
			v2pa(U64(us_lvl2_table));
}

void mmu_update_user_mapping(struct mmap *mmap)
{

	/* No user process will need more than 2mb for now. Therefore only altering idx [0].
	 * In the ARM architecture we have to use the 'break before make' sequennce , i.e.,
	 * we can't just modify a page table entry directly but have to override it first. */

	/* Break before... */
	us_lvl2_table[0] = 0;
	__asm__ volatile (
	"dc	cvau, %0\n"
	"dsb	ish\n"
	"tlbi	vae1is, %0\n"
	"dsb	ish\n"
	"ic	ialluis\n"
	:: "r" (&us_lvl2_table[0]));

	/* ... make. */
	us_lvl2_table[0] = TT_S1_TABLE |
			   U64(v2pa(mmap->table));
	__asm__ volatile (
	"dsb	ish\n"
	"isb"
	:: );
}

/*
 * codep and datap have to be physicall addresses
 */
struct mmap* mmu_create_user_mapping(uint64_t *codep, uint64_t *stackp)
{
	struct mmap *mmap = (struct mmap*) get_free_virt_kpage(); // TODO: that's a waste
	memzero((void*) mmap, PAGE_SIZE);
	mmap->table = (uint64_t *) get_free_virt_kpage();
	//TODO: memzero table

	/* Code goes to virt. 0x0. */
	mmap->table[0] =
		TT_S1_NORMAL_WBWA |
		TT_S1_INNER_SHARED |
		TT_S1_PAGE |
		//TT_S1_PAGE_UXN |
		TT_S1_PAGE_EL0_RW | // TODO: Make readonly
		U64(codep);
	/* Stack goes to virt. 0x1000*/
	mmap->table[1] =
		TT_S1_NORMAL_WBWA |
		TT_S1_INNER_SHARED |
		TT_S1_PAGE |
		TT_S1_PAGE_UXN |
		TT_S1_PAGE_EL0_RW |
		U64(stackp);

	return mmap;
}

void mmu_init()
{
	/* Set translation table base register 0 (EL 1). */
	_set_ttbr0_el1((uint64_t *) v2pa(U64(lvl0_table)));
	/* Set memory attribute indirection register (EL 1). */
	_set_mair_el1(MAIR_EL1);
	/* Set translation control register (EL 1). */
	_set_tcr_el1(
		(16)	  | /* T0SZ = VA space is 48 bits. */
		(0x1<<8)  | /* IRGN0 = 0b01. Walks to TTBR0 are Inner WB/WA. */
		(0x1<<10) | /* ORGN0 = 0b01. Walks to TTBR0 are Outer WB/WA. */
		(0x3<<12) | /* SH0 = 0b11. Inner Sharable. */
		(0x1<<23)   /* EPD1 = 0b01. Disable table walks from TTBR1 */
			    /* TBI0 = 0b0. Top byte not ignored. */
			    /* TG0 = 0b00. 4KB granule. */
			    /* IPS = 0. 32-bit PA space. */
		);
}


void mmu_enable()
{
	__asm__ volatile (
		"dsb	sy\n"
		"mov 	x0, #(1 << 0)\n"   	// M=1 Enable the stage 1 MMU
		"orr	x0, x0, #(1 << 2)\n" 	// C=1 Enable data and unified caches
		"orr	x0, x0, #(1 << 12)\n"   // I=1 Enable instruction fetches to allocate into unified caches
		"msr	sctlr_el1, x0\n"
		"isb" ::);
}

void mmu_flush_tlb_e3()
{
	__asm__ volatile (
		"dsb	sy\n"
		"tlbi	alle3\n"	// M=1 Enable the stage 1 MMU
		"dsb	ish\n"		// C=1 Enable data and unified caches
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

static void _set_ttbr1_el1(uint64_t * ttrbr1)
{
	__asm__ volatile ("msr	ttbr1_el1, %0" :: "r" (ttrbr1));
}

static void _set_mair_el1(uint64_t mair_el1)
{
	__asm__ volatile ("msr	mair_el1, %0" :: "r" (mair_el1));
}
