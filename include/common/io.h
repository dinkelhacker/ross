#ifndef IO_H
#define IO_H
#include <stdbool.h>
#include <stdint.h>

extern void uart_init();
extern void uart_print(char *buffer);
extern void uart_do_print(char *buffer);
extern unsigned int uart_read_u32();
extern void uart_sendInt(unsigned int num);
extern unsigned char uart_read_byte_blocking();
extern int uart_read_byte(char * byte);
extern unsigned int uart_is_read_rdy();
extern void uart_writeByteBlocking(unsigned char ch);
extern void mmio_write32(uint64_t reg, uint32_t val);
extern unsigned int mmio_read32(uint64_t reg);
extern bool readString(char *buff);

#endif
