#include "exception_types.h"

// This macro is used to create an entry in the vector table.
// In Aarch64 vectors are each 0x80 (2^7) bytes apart.

.macro vector_entry label
	.align 7 
	b \label
.endm

.macro handle_exception type handler el
	store_processor_state \el
	mov  x0, #\type
	mrs  x1, esr_el1
	mrs  x2, elr_el1 
	clear_esr
	bl \handler 
	restore_processor_state \el
.endm

.macro clear_esr
	msr esr_el1, xzr
.endm

/*
 * Store the processor state on the stack
 *       ________
 *      |spsr_el1|  
 *      |elr_el1 |  
 *      |  x30   |  
 *      |   ..   |
 *      |   x6   |
 *      |   x5   | 
 *      |   x4   |
 *      |   x3   |
 *      |   x2   |
 *      |   x1   |
 *      |   x0   | <- sp
 *       --------
 */

.macro store_processor_state el
	// make space for the frame
	sub  sp, sp, #STORED_FRAME_SIZE

	//push xn and xn+1 on the stack
	stp  x0, x1, [sp, #16 * 0]
	stp  x2, x3, [sp, #16 * 1]
	stp  x4, x5, [sp, #16 * 2]
	stp  x6, x7, [sp, #16 * 3]
	stp  x8, x9, [sp, #16 * 4]
	stp  x10, x11, [sp, #16 * 5]
	stp  x12, x13, [sp, #16 * 6]
	stp  x14, x15, [sp, #16 * 7]
	stp  x16, x17, [sp, #16 * 8]
	stp  x18, x19, [sp, #16 * 9]
	stp  x20, x21, [sp, #16 * 10]
	stp  x22, x23, [sp, #16 * 11]
	stp  x24, x25, [sp, #16 * 12]
	stp  x26, x27, [sp, #16 * 13]
	stp  x28, x29, [sp, #16 * 14]

	.if	\el == 0
	mrs	x21, sp_el0
	.else
	add	x21, sp, #STORED_FRAME_SIZE
	.endif

	// store pc of the interrupted task in x22
	mrs	x22, elr_el1
	// store saved program status register in x23
	mrs	x23, spsr_el1

	stp	x30, x21, [sp, #16 * 15] 
	stp	x22, x23, [sp, #16 * 16] 
.endm


.macro  restore_processor_state el
	// get saved program status
	ldp	x22, x23, [sp, #16 * 16]
	// get pc of the previously interrupted task (x22) and x30
	ldp	x30, x21, [sp, #16 * 15] 

	.if	\el == 0
	msr	sp_el0, x21
	.endif

	// put address in exception link register
	msr	elr_el1, x22
	// restore program status
	msr	spsr_el1, x23

	// restore all the registers
	ldp  x0, x1, [sp, #16 * 0]
	ldp  x2, x3, [sp, #16 * 1]
	ldp  x4, x5, [sp, #16 * 2]
	ldp  x6, x7, [sp, #16 * 3]
	ldp  x8, x9, [sp, #16 * 4]
	ldp  x10, x11, [sp, #16 * 5]
	ldp  x12, x13, [sp, #16 * 6]
	ldp  x14, x15, [sp, #16 * 7]
	ldp  x16, x17, [sp, #16 * 8]
	ldp  x18, x19, [sp, #16 * 9]
	ldp  x20, x21, [sp, #16 * 10]
	ldp  x22, x23, [sp, #16 * 11]
	ldp  x24, x25, [sp, #16 * 12]
	ldp  x26, x27, [sp, #16 * 13]
	ldp  x28, x29, [sp, #16 * 14]
	
	// move stack pointer up since we popped a lot of stuff
	add  sp, sp, #STORED_FRAME_SIZE

	// return from exception (pc we copied into elr_el1)
	eret
.endm

// Exception vectors

.align  11
.globl vectors 
vectors:
	vector_entry  sync_el1t       // Synchronous EL1t
	vector_entry  irq_el1t        // IRQ EL1t
	vector_entry  fiq_el1t        // FIQ EL1t
	vector_entry  serror_el1t     // Error EL1t

	vector_entry  sync_el1h       // Synchronous EL1h
	vector_entry  irq_el1h        // IRQ EL1h
	vector_entry  fiq_el1h        // FIQ EL1h
	vector_entry  serror_el1h     // Error EL1h

	vector_entry  sync_el0_64     // Synchronous 64-bit EL0
	vector_entry  irq_el0_64      // IRQ 64-bit EL0
	vector_entry  fiq_el0_64      // FIQ 64-bit EL0
	vector_entry  serror_el0_64   // Error 64-bit EL0

	vector_entry  sync_el0_32     // Synchronous 32-bit EL0
	vector_entry  irq_el0_32      // IRQ 32-bit EL0
	vector_entry  fiq_el0_32      // FIQ 32-bit EL0
	vector_entry  serror_el0_32   // Error 32-bit EL0




// EL1t
sync_el1t:
	handle_exception SYNC_EL1T, invalid_exception, 1 

irq_el1t:
	handle_exception IRQ_EL1T, invalid_exception, 1

fiq_el1t:
	handle_exception FIQ_EL1T, invalid_exception, 1

serror_el1t:
	handle_exception SERROR_EL1T,invalid_exception, 1


// EL1h
sync_el1h:
	store_processor_state 1
	mrs  x0, esr_el1
	mrs  x1, elr_el1 
	clear_esr
	mov x2, x8
	bl handle_sync
	restore_processor_state 1

irq_el1h:
	handle_exception IRQ_EL1H, handle_irq, 1

fiq_el1h:
	handle_exception FIQ_EL1H, invalid_exception, 1

serror_el1h:
	handle_exception SERROR_EL1H, invalid_exception, 1


// EL0_64
sync_el0_64:
	store_processor_state 0
	mrs x0, esr_el1
	mrs x1, elr_el1 
	clear_esr
	mov x2, x8
	bl handle_sync
	restore_processor_state 0

irq_el0_64:
	handle_exception IRQ_EL0_64, handle_irq, 0

fiq_el0_64:
	handle_exception FIQ_EL0_64, invalid_exception, 0

serror_el0_64:
	handle_exception SERROR_EL0_64, invalid_exception, 0


// EL0_32
sync_el0_32:
	handle_exception SYNC_EL0_32, invalid_exception, 0

irq_el0_32:
	handle_exception IRQ_EL0_32, invalid_exception, 0

fiq_el0_32:
	handle_exception FIQ_EL0_32, invalid_exception, 0

serror_el0_32:
	handle_exception SERROR_EL0_32, invalid_exception, 0

// Set vector table base address
.section ".text"
.globl init_vector_table
init_vector_table:
	adr x0, vectors
	msr vbar_el1, x0
	ret

// Enable all interrupts
.globl enable_irq
enable_irq:
	msr daifclr, #2
	ret

// Disable all interrupts
.globl disable_irq
disable_irq:
	msr daifset, #2
	ret

.globl ret_from_fork
ret_from_fork:
	// enable preemption
	bl    schedule_tail
	// restore argument
	mov    x0, x20
	// bye bye...
	blr    x19
ret_to_user:
	bl disable_irq				
	restore_processor_state 0 
