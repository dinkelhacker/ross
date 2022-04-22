.section ".text"
.equ TT_S1_DEVICE_nGnRnE,   0x00600000000000409    // Index = 2, AF=1, PXN=1, UXN=1
.equ TT_S1_NORMAL_NO_CACHE, 0x00000000000000401    // Index = 0, AF=1
.equ TT_S1_NORMAL_WBWA,     0x00000000000000405    // Index = 1, AF=1
.equ TT_S1_OUTER_SHARED,    (2 << 8)               // Outer-shareable
.equ TT_S1_INNER_SHARED,    (3 << 8)               // Inner-shareable

.globl mmu_init
mmu_init:
	ldr	x0, =tt_l1_base
	msr	ttbr0_el1, x0


	/* Set up memory attributes
	 * 
	 * This equates to:
	 * 0 = b01000100 = Normal, Inner/Outer Non-Cacheable
	 * 1 = b11111111 = Normal, Inner/Outer WB/WA/RA
	 * 2 = b00000000 = Device-nGnRnE
 	*/
 	mov   x0, #0x000000000000FF44
 	msr   mair_el1, x0


	// Set up TCR_EL1
	// ---------------
	mov	x0, #0x19            // T0SZ=0b011001 Limits VA space to 39 bits,
	orr	x0, x0, #(0x1 << 8)  // IGRN0=0b01  Walks to TTBR0 are Inner WB/WA
	orr	x0, x0, #(0x1 << 10) // OGRN0=0b01  Walks to TTBR0 are Outer WB/WA
	orr	x0, x0, #(0x3 << 12) // SH0=0b11   Inner Shareable
	orr	x0, x0, #(0x1 << 23) // EPD1=0b1   Disable table walks from TTBR1
	                           // TBI0=0b0   Top byte not ignored
	                           // TG0=0b00   4KB granule
	                           // IPS=0     32-bit PA space
	msr   tcr_el1, x0

	// Invalidate TLBs
	// ----------------
	tlbi	vmalle1 
	dsb	sy
	isb
	
	
	ldr   x1, =tt_l1_base              // Address of L1 table

	// [0]: 0x0000,0000 - 0x3FFF,FFFF
	ldr	x0, =TT_S1_NORMAL_WBWA 	// Entry template
	orr	x0, x0, #TT_S1_INNER_SHARED  // ‘OR' with inner-shareable attribute
	                                   // AP=0, RW
	                                   // Don't need to OR in address, as it is 0
	str	x0, [x1]

	// [1]: 0x4000,0000 - 0x7FFF,FFFF
	ldr	x0, =TT_S1_NORMAL_NO_CACHE
	orr	x0, x0, #TT_S1_INNER_SHARED  // ‘OR' with inner-shareable attribute
	                                   // AP=0, RW
	orr	x0, x0, #0x40000000          // 'OR' template with base physical address
	str	x0, [x1, #8]
	
	// [2]: 0x8000,0000 - 0xBFFF,FFFF (DRAM on the VE and Base Platform)
	ldr	x0, =TT_S1_NORMAL_WBWA
	orr	x0, x0, #TT_S1_INNER_SHARED  // ‘OR' with inner-shareable attribute
	                                   // AP=0, RW
	orr	x0, x0, #0x80000000          // 'OR' template with base physical address
	str	x0, [x1, #16]
	
	// [3]: 0xC000,0000 - 0xFFFF,FFFF (DRAM on the VE and Base Platform)
	ldr	x0, =TT_S1_DEVICE_nGnRnE // Entry template
	//orr	x0, x0, #TT_S1_OUTER_SHARED  // ‘OR' with inner-shareable attribute
	                                   // AP=0, RW
	orr	x0, x0, #0xC0000000          // 'OR' template with base physical address
	
	str	x0, [x1, #24]
	
	dsb	sy
	
	// enable mmu
	mov 	x0, #(1 << 0)                     // M=1           Enable the stage 1 MMU
	orr	x0, x0, #(1 << 2)                 // C=1           Enable data and unified caches
	orr	x0, x0, #(1 << 12)                // I=1           Enable instruction fetches to allocate into unified caches
	                                           // A=0           Strict alignment checking disabled
	                                           // SA=0          Stack alignment checking disabled
	                                           // WXN=0         Write permission does not imply XN
	                                           // EE=0          EL3 data accesses are little endian
	msr	sctlr_el1, x0
	isb
	ret

.section TT,"ax"
.align 12

.global tt_l1_base
tt_l1_base:
.fill 4096,1,0
