#include "io.h"
#include "gpio.h"
#include "peripherals.h"
#include <stdbool.h>
#include "error.h"
#include "stdint.h"
#include "spinlock.h"

//__attribute__((section (".locks")))
static spinlock_t uart_lock;

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init()
{
	mmio_write32(AUX_ENABLES, 1); //enable UART1
	mmio_write32(AUX_MU_IER_REG, 0);
	mmio_write32(AUX_MU_CNTL_REG, 0);
	mmio_write32(AUX_MU_LCR_REG, 3); //8 bits
	mmio_write32(AUX_MU_MCR_REG, 0);
	mmio_write32(AUX_MU_IER_REG, 0);
	mmio_write32(AUX_MU_IIR_REG, 0xC6); //disable interrupts
	mmio_write32(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
	gpio_set_pull(14, GPIO_P_NONE);
	gpio_set_pull(15, GPIO_P_NONE);
	gpio_function_select(14, GPIO_F_ALT5);
	gpio_function_select(15, GPIO_F_ALT5);
	mmio_write32(AUX_MU_CNTL_REG, 3); //enable RX/TX
}

uint32_t uart_isWriteByteReady()
{
	return mmio_read32(AUX_MU_LSR_REG) & 0x20;
}

uint32_t uart_is_read_rdy(){
	return mmio_read32(AUX_MU_LSR_REG) & 0x01;
}

uint32_t uart_read_u32()
{
	uint32_t num;
	for (int i=0; i<4;i++) {
		char c = uart_read_byte_blocking();
		num = num << 8;
		num += (uint32_t) c;
	}
	return num;
}

void uart_sendInt(uint32_t num)
{
	uart_writeByteBlocking((char) ((num >> 24)));
	uart_writeByteBlocking((char) ((num >> 16)));
	uart_writeByteBlocking((char) ((num >> 8)));
	uart_writeByteBlocking((char) ((num)));
}

void uart_writeByteBlocking(unsigned char ch)
{
	while (!uart_isWriteByteReady()); 
	mmio_write32(AUX_MU_IO_REG, (uint32_t)ch);
}


void uart_print(char *buffer)
{
	spin_lock(&uart_lock);
	uart_do_print(buffer);
	spin_unlock(&uart_lock);
}

void uart_do_print(char *buffer)
{
	while (*buffer) {
		 if (*buffer == '\n') 
			 uart_writeByteBlocking('\r');
		 uart_writeByteBlocking(*buffer++);
	}
}

unsigned char uart_read_byte_blocking()
{
	while (!uart_is_read_rdy());
	return (unsigned char)mmio_read32(AUX_MU_IO_REG);
}


int uart_read_byte(char* byte)
{
	if (uart_is_read_rdy()) {
		*byte = (unsigned char)mmio_read32(AUX_MU_IO_REG);
		return OK;
	} else {
		return ERROR;
	}
}

bool readString(char *buff)
{
	unsigned char c;
	while(*buff) {
		c = uart_read_byte_blocking();
		if (c != *buff) return false;
		buff++;
	}
	return true;
}
