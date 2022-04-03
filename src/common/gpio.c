#include <stdint.h>
#include "error.h"
#include "io.h"
#include "gpio.h"
#include "peripherals.h"

static const uint32_t fs_regs[6] = 
	{GPFSEL0, GPFSEL1, GPFSEL2, GPFSEL3, GPFSEL4, GPFSEL5};
static const uint32_t set_regs[2] = {GPSET0, GPSET1};
static const uint32_t clear_regs[2] = {GPCLR0, GPCLR1};
static const uint32_t pull_regs[4] = 
	{GPPUPPDN0, GPPUPPDN1, GPPUPPDN2, GPPUPPDN3};
static const uint32_t lvl_regs[2] = {GPLEV0, GPLEV1};
static const uint32_t eds_regs[2] = {GPEDS0, GPEDS1};
static const uint32_t ren_regs[2] = {GPREN0, GPREN1};
static const uint32_t fen_regs[2] = {GPFEN0, GPFEN1};
static const uint32_t aren_regs[2] = {GPAREN0, GPAREN1};
static const uint32_t afen_regs[2] = {GPAFEN0, GPAFEN1};
static const uint32_t hen_regs[2] = {GPHEN0, GPHEN1};
static const uint32_t len_regs[2] = {GPLEN0, GPLEN1};

void set_bfield_in_reg(
	uint32_t reg, 	uint32_t field_size,
	uint32_t value, uint32_t shift)
{
	uint32_t field_mask = (1 << field_size) - 1;
	uint32_t regval = mmio_read(reg);
	regval &= ~((uint32_t) field_mask << shift);
	regval |= ((uint32_t) value << shift);
	mmio_write(reg, regval);
}

int gpio_function_select(uint8_t pin, gpio_function fun)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg(fs_regs[pin/10], 3, fun, (pin % 10) * 3);
	return OK;
}

int gpio_set_pin(uint8_t pin)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg(set_regs[pin/32], 1, 1, pin % 32);
	return OK;
}

int gpio_clear_pin(uint8_t pin)
{
	if(pin > 57)
		return ERROR; 
	set_bfield_in_reg(clear_regs[pin/32], 1, 1, pin % 32);
	return OK;
}

int gpio_set_pull(uint8_t pin, gpio_pull pull)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg(pull_regs[pin/16], 2, (uint32_t) pull, (pin % 16) * 2);
	return OK;
}

int gpio_get_lvl(uint8_t pin)
{
	if(pin > 57)
		return ERROR;
	return 0x00000001u & (mmio_read(lvl_regs[pin/32]) >> (pin % 32));
}

int gpio_clear_event(uint8_t pin)
{
	if(pin > 57)
		return ERROR;
	set_bfield_in_reg(eds_regs[pin/32], 1, 1, pin % 32);
	return OK;
}

int gpio_rising_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg(ren_regs[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_falling_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg(fen_regs[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_high_level_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg(hen_regs[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_low_level_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg(len_regs[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_async_rising_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg(aren_regs[pin/32], 1, status, pin % 32);
	return OK;
}

int gpio_async_falling_edge_detect(uint8_t pin, uint32_t status)
{
	if(pin >57 || status > 1)
		return ERROR;
	set_bfield_in_reg(afen_regs[pin/32], 1, status, pin % 32);
	return OK;
}
