#include "syscalls.h"
.globl task_suspend
task_suspend:
  ldr  x8, =SYSC_TASK_SUSPEND
  svc  #0x0
