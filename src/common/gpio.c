#include <stdint.h>
#include "error.h"
#include "io.h"
#include "gpio.h"
#include "peripherals.h"

static volatile gpio_t *gpio = (gpio_t *) GPIO_BASE;

void set_bfield_in_reg(
	uint64_t reg, 	uint32_t field_size,
	uint32_t value, uint32_t shift)
{
	uint32_t field_mask = (1 << field_size) - 1;
	uint32_t regval = mmio_read32(reg);
	regval &= ~((uint32_t) field_mask << shift);
	regval |= ((uint32_t) value << shift);
	mmio_write32(reg, regval);
}

int gpio_function_select(uint8_t pin, gpio_function fun)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gpfsel[pin/10], 3, fun, (pin % 10) * 3);
	return OK;
}

int gpio_set_pin(uint8_t pin)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gpset[pin/32], 1, 1, pin % 32);
	return OK;
}

int gpio_clear_pin(uint8_t pin)
{
	if(pin > 57)
		return ERROR; 
	set_bfield_in_reg((uint64_t) &gpio->gpclr[pin/32], 1, 1, pin % 32);
	return OK;
}

int gpio_set_pull(uint8_t pin, gpio_pull pull)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gppull[pin/16], 2, (uint32_t) pull, (pin % 16) * 2);
	return OK;
}

int gpio_get_lvl(uint8_t pin)
{
	if(pin > 57)
		return ERROR;
	return 0x00000001u & (mmio_read32((uint64_t) &gpio->gplev[pin/32]) >> (pin % 32));
}

int gpio_clear_event(uint8_t pin)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gpeds[pin/32], 1, 1, pin % 32);
	return OK;
}

int gpio_rising_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gpren[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_falling_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gpfen[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_high_level_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gphen[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_low_level_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gplen[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_async_rising_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gparen[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_async_falling_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg((uint64_t) &gpio->gpafen[pin/32], 1, status, pin % 32);
	return OK;
}
