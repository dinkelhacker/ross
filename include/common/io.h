#ifndef IO_H
#define IO_H
#include <stdbool.h>
#include <stdint.h>

extern void uart_init();
extern void uart_writeText(char *buffer);
extern unsigned int uart_readIntBlocking();
extern void uart_sendInt(unsigned int num);
extern unsigned char uart_readByteBlocking();
extern int uart_readByte(char * byte);
extern unsigned int uart_isReadByteReady();
extern void uart_writeByteBlocking(unsigned char ch);
extern void mmio_write(long reg, unsigned int val);
extern unsigned int mmio_read(long reg);
extern bool readString(char *buff);

#endif
