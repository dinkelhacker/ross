#include "io.h"
#include "peripherals.h"
#include <stdbool.h>
#include "error.h"


#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

// Basic mmio read/write
void mmio_write(long reg, unsigned int val)
{
	*(volatile unsigned int *) reg = val;
}

unsigned int mmio_read(long reg)
{
	return *(volatile unsigned int *) reg;
}

unsigned int gpio_call(unsigned int pin_number, unsigned int value, unsigned int base, unsigned int field_size, unsigned int field_max)
{
	unsigned int field_mask = (1 << field_size) - 1;

	if (pin_number > field_max) return 0;
	if (value > field_mask) return 0; 

	unsigned int num_fields = 32 / field_size;
	unsigned int reg = base + ((pin_number / num_fields) * 4);
	unsigned int shift = (pin_number % num_fields) * field_size;

	unsigned int curval = mmio_read(reg);
	curval &= ~(field_mask << shift);
	curval |= value << shift;
	mmio_write(reg, curval);

	return 1;
}

unsigned int gpio_set(unsigned int pin_number, unsigned int value)
{
	return gpio_call(pin_number, value, GPSET0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_clear(unsigned int pin_number, unsigned int value)
{
	return gpio_call(pin_number, value, GPCLR0, 1, GPIO_MAX_PIN);
}

unsigned int gpio_pull(unsigned int pin_number, unsigned int value)
{
	return gpio_call(pin_number, value, GPPUPPDN0, 2, GPIO_MAX_PIN);
}

unsigned int gpio_function(unsigned int pin_number, unsigned int value)
{
	return gpio_call(pin_number, value, GPFSEL0, 3, GPIO_MAX_PIN);
}

void gpio_useAsAlt5(unsigned int pin_number)
{
	gpio_pull(pin_number, Pull_None);
	gpio_function(pin_number, GPIO_FUNCTION_ALT5);
}


void uart_init()
{
	mmio_write(AUX_ENABLES, 1); //enable UART1
	mmio_write(AUX_MU_IER_REG, 0);
	mmio_write(AUX_MU_CNTL_REG, 0);
	mmio_write(AUX_MU_LCR_REG, 3); //8 bits
	mmio_write(AUX_MU_MCR_REG, 0);
	mmio_write(AUX_MU_IER_REG, 0);
	mmio_write(AUX_MU_IIR_REG, 0xC6); //disable interrupts
	mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));
	gpio_useAsAlt5(14);
	gpio_useAsAlt5(15);
	mmio_write(AUX_MU_CNTL_REG, 3); //enable RX/TX
}

unsigned int uart_isWriteByteReady()
{
	return mmio_read(AUX_MU_LSR_REG) & 0x20;
}

unsigned int uart_isReadByteReady(){
	return mmio_read(AUX_MU_LSR_REG) & 0x01;
}

unsigned int uart_readIntBlocking()
{
	unsigned int num;
	for (int i=0; i<4;i++) {
		char c = uart_readByteBlocking();
		num = num << 8;
		num += (unsigned int) c;
	}
	return num;
}

void uart_sendInt(unsigned int num)
{
	uart_writeByteBlocking((char) ((num >> 24)));
	uart_writeByteBlocking((char) ((num >> 16)));
	uart_writeByteBlocking((char) ((num >> 8)));
	uart_writeByteBlocking((char) ((num)));
}

void uart_writeByteBlocking(unsigned char ch)
{
	while (!uart_isWriteByteReady()); 
	mmio_write(AUX_MU_IO_REG, (unsigned int)ch);
}

void uart_writeText(char *buffer)
{
	while (*buffer) {
		 if (*buffer == '\n') 
			 uart_writeByteBlocking('\r');
		 uart_writeByteBlocking(*buffer++);
	}
}

unsigned char uart_readByteBlocking()
{
	while (!uart_isReadByteReady());
	return (unsigned char)mmio_read(AUX_MU_IO_REG);
}


int uart_readByte(char* byte)
{
	if (uart_isReadByteReady()) {
		*byte = (unsigned char)mmio_read(AUX_MU_IO_REG);
		return OK;
	} else {
		return ERROR;
	}
}

bool readString(char *buff)
{
	unsigned char c;
	while(*buff) {
		c = uart_readByteBlocking();
		if (c != *buff) return false;
		buff++;
	}
	return true;
}
