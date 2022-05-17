// AArch64 mode

#include "memory_layout.h"

// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.globl _start
// Entry point for the kernel. Registers:
// x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
// x1 -> 0
// x2 -> 0
// x3 -> 0
// x4 -> 32 bit kernel entry point, _start location

_start:
#ifdef DEBUG_STARTUP
	b _start
	mrs x3, CurrentEL
#endif
	/* Enable EL3 MMU.
	 * Table os already setup by bootloader.*/
	dsb 	sy
	mov 	x0, #(1 << 0)   	// M=1 Enable the stage 1 MMU
	orr	x0, x0, #(1 << 2) 	// C=1 Enable data and unified caches
	orr	x0, x0, #(1 << 12)   	// I=1 Enable instruction fetches to allocate into unified caches
	msr	sctlr_el3, x0
	isb

	/* Check processor ID is zero (executing on main core), else hang.
	 * Copy system register `mpidr_el1` into x1. */
	mrs 	x1, mpidr_el1
	and 	x1, x1, #3			// and x1 with 0b11
	cmp 	x1, #0
	ldr 	x5, =__stack_core_0
	adr 	x6, _entry_el3_core0
	beq 	1f
	adr 	x6, _entry_el3
	cmp 	x1, #1
	ldr 	x5, =__stack_core_1
	beq 	1f
	cmp 	x1, #2
	ldr 	x5, =__stack_core_2
	beq 	1f
	cmp 	x1, #3
	ldr 	x5, =__stack_core_3
1:
	mov 	x7, #KERNEL_VA_OFFSET
	add 	x6, x6, x7
	br	x6


_halt:  
	// `WaitForEvent` suspend the core until an event occures 
	wfe
	b	_halt 

.globl _conf_sctlr_el1
_conf_sctlr_el1:
	/*****************************************************************************
	 * [SCTLR_EL1] `System Control Register Exception Level 1`
	 ****************************************************************************/
	
	// clear the register
	mov	w1, #0

	// Implicitly:
	// - disable instruction cache #(0 << 12) 
	// - disable data cache #(0 << 2)
	// - disable MMU for now #(0 << 0)
	// - set EE (enidianness of data access at EL 1) bit to 0 (little-endian)
	// - set E0E (enidianness of data access at EL 0) bit to 0 (little-endian)
	
	// set RES1 bits
	orr	w1, w1, #(1 << 29)
	orr	w1, w1, #(1 << 28)
	orr	w1, w1, #(1 << 23)
	orr	w1, w1, #(1 << 22)
	orr	w1, w1, #(1 << 20)
	orr	w1, w1, #(1 << 11)

	// set config
	msr   sctlr_el1, x1
	ret

.globl _conf_hcr_el2
_conf_hcr_el2:
	/*****************************************************************************
	 * [HCR_EL2] `Hypervisor Configuration Register (EL2)`
	 ****************************************************************************/

	// Set execution state at EL1 to Aarch64 (1 << 31)
	movz	x0, 0x8000, lsl 16

	// set config
	msr	hcr_el2, x0
	ret

	
.globl _conf_scr_el3
_conf_scr_el3:
	/*****************************************************************************
	 * [SCR_EL3] `Secure Configuration Register`
	 ****************************************************************************/

	// clear the register
	mov	w1, #0

	// set RW bit (Execution state for lower Exception levels is Aarch64)
	ORR	w1, w1, #(1 << 10)

	// set NS bit (EL0 & EL1 are non-secure)
	ORR	w1, w1, #(1 << 0) 
	
	// set RES1 bits
	ORR	w1, w1, #(1 << 5)
	ORR	w1, w1, #(1 << 4)
	
	// set config
	msr	scr_el3, x1
	ret

.globl _conf_spsr_el3
_conf_spsr_el3:
	/*****************************************************************************
	 * [SPSR_EL3] `Saved Program Status Register (EL3)`
	 ****************************************************************************/

	// clear the register
	mov	w1, #0

	// mask SError interrupts
	orr	w1, w1, #(1 << 8)
	
	// mask IRQ interrupt
	orr	w1, w1, #(1 << 7)

	// mask FIQ interrupt
	orr	w1, w1, #(1 << 6)

	// AArch64 state (Exception level and selected SP) that an exception was taken from.
	// 0b0101 -> (EL1h)
	orr	w1, w1, #(1 << 2)
	orr	w1, w1, #(1 << 0)
	
	// set config
	msr	spsr_el3, x1

	/*****************************************************************************
	* [ELR_EL3] `Exception Link Register (EL3)`
	*****************************************************************************/
	
	// write PC relative address of `_entry` into x0
	adr	x0, _kernel_entry_el1
	
	// set config
	msr	elr_el3, x0

	// invalidate tlb at el1
	dsb	sy
	tlbi	alle1
	dsb	sy
	// change exception level and jump to address in `elr_el3`
	eret 

// else
.globl _conf_spsr_el2
_conf_spsr_el2:
	/*****************************************************************************
	 * [SPSR_EL2] `Saved Program Status Register (EL2)`
	 ****************************************************************************/

	// clear the register
	mov	w1, #0

	// mask SError interrupts
	orr	w1, w1, #(1 << 8)
	
	// mask IRQ interrupt
	orr	w1, w1, #(1 << 7)

	// mask FIQ interrupt
	orr	w1, w1, #(1 << 6)

	// AArch64 state (Exception level and selected SP) that an exception was taken from.
	// 0b0101 -> (EL1h)
	orr	w1, w1, #(1 << 2)
	orr	w1, w1, #(1 << 0)
	
	// set config
	msr	spsr_el2, x1

	/*****************************************************************************
	* [ELR_EL2] `Exception Link Register (EL2)`
	*****************************************************************************/
	
	// write PC relative address of `_entry` into x0
	adr	x0, _kernel_entry_el1
	
	// set config
	msr	elr_el2, x0

	// change exception level and jump to address in `elr_el2`
	eret 

_kernel_entry_el1:
	/* Clean and invalidate cashes. */
	dsb	sy
	ic	IALLUIS
	dsb	sy

	mrs 	x1, mpidr_el1
	and 	x1, x1, #3

	cmp 	x1, #0
	ldr 	x5, =__stack_core_0
	mov 	  sp, x5
	beq 	os_entry
	cmp 	x1, #1
	ldr 	x5, =__stack_core_1
	mov  	sp, x5
	beq 	os_entry_secondary
	cmp 	x1, #2
	ldr 	x5, =__stack_core_2
	mov 	sp, x5
	beq 	os_entry_secondary
	cmp 	x1, #3
	ldr 	x5, =__stack_core_3
	mov 	sp, x5
	beq 	os_entry_secondary


_entry_el3_core0:
	// clear bss
	ldr	x6, =__bss_start
	ldr	w7, =__bss_size
1: 
	cbz	w7, 2f
	str	xzr, [x6], #8
	sub	w7, w7, #1
	cbnz	w7, 1b
 
2:  
_entry_el3: 
	// Set stack
	mov	sp, x5

	// jump to C code, should not return
	bl	init
	// if it returns halt this core too
	b	_halt
