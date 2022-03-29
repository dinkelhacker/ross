#ifndef EXCEPTIONS_H 
#define EXCEPTIONS_H 
extern void init_vector_table(void);
extern void enable_irq(void);
extern void disable_irq(void);
extern uint32_t get_syscall_id(void);
#endif
