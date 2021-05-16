// AArch64 mode
 
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
  // Check processor ID is zero (executing on main core), else hang
  // Copy system register `mpidr_el1` into x1
  mrs     x1, mpidr_el1
  // and x1 with 0b11
  and     x1, x1, #3
  // If core `0` go to `_mainCore`
  cbz     x1, _mainCore
  
_halt:  
    // `WaitForEvent` suspend the core until an event occures 
    wfe
    b      _halt 

_mainCore: 
  // Main core
  // Set stack before our code
  ldr   x5, =_start
  mov   sp, x5
  
  // clear bss
  ldr   x5, =__bss_start
  ldr   w6, =__bss_size
1: 
  cbz   w6, 2f
  str   xzr, [x5], #8
  sub   w6, w6, #1
  cbnz  w6, 1b
 
  // jump to C code, should not return
2:  bl    bootloader_main
  // for failsafe, halt this core too
  b _halt
