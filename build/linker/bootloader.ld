#include "memory_layout.h"
ENTRY(_start)
 
SECTIONS
{
    . = CORE_RELEASE;
    __core_start = .;
    .core_release_block CORE_RELEASE:
    {
	KEEP(*(.core_release_section))
    }
    __core_end= .;


    _stack_core0 = BOOTLOADER_BASE;
    _stack_core1 = BOOTLOADER_BASE - 0x2000;
    _stack_core2 = BOOTLOADER_BASE - 0x4000;
    _stack_core3 = BOOTLOADER_BASE - 0x6000;
    /* Starts at BOOTLOADER_BASE. */
    . = BOOTLOADER_BASE;
    __start = .;
    __text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
    __text_end = .;
 
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
    __end = .;
}
