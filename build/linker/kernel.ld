#include "memory_layout.h"
ENTRY(_start)


SECTIONS
{
	/* Stacks for init tasks */
	__stack_core_0 = KERNEL_STACK_CORE0;
	__stack_core_1 = KERNEL_STACK_CORE1;
	__stack_core_2 = KERNEL_STACK_CORE2;
	__stack_core_3 = KERNEL_STACK_CORE3;

	/* Starts at KERNEL_BASE. */
	. = KERNEL_BASE;
	__start = .;
	__text_start = .;
	.text :
	{
	    KEEP(*(.text.boot))
	    __user_process_start = .;
	    *(.text.user_process)
	    __user_process_end = .;
	    *(.text)
	    . = ALIGN(4096);
	    __user_syscalls_start = .;
	    *(.text.syscalls)
	    . = ALIGN(4096);
	    __user_syscalls_end = .;
	}
	. = ALIGN(4096); /* align to page size */

	__rodata_start = .;
	.rodata :
	{
	    *(.rodata)
	}
	. = ALIGN(4096); /* align to page size */
	__rodata_end = .;

	__data_start = .;
	.data :
	{
	    *(.data)
	}
	. = ALIGN(4096); /* align to page size */
	__data_end = .;

	__bss_start = .;
	.bss :
	{
	    bss = .;
	    *(.bss)
	}
	. = ALIGN(4096); /* align to page size */
	__bss_end = .;
	__bss_size = __bss_end - __bss_start;

	. = ALIGN(4096); /* align to page size */
	__end = .;
}
