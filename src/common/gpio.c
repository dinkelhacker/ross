#include <stdint.h>
#include "io.h"
#include "gpio.h"
#include "peripherals.h"

static const uint32_t fs_regs[5] = {GPFSEL0, GPFSEL1, GPFSEL2,
					GPFSEL3, GPFSEL4, GPFSEL5};
static const uint32_t set_regs[2] = {GPSET0, GPSET1};
static const uint32_t clear_regs[2] = {GPCLR0, GPCLR1};
static const uint32_t pull_regs[4] = {GPPUPPDN0, GPPUPPDN1, GPPUPPDN2, GPPUPPDN3};
static const uint32_t lvl_regs[2] = {GPLEV0, GPLEV1};

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


void gpio_function_select(uint8_t pin, gpio_function fun)
{
	if(pin > 57)
		return; // Error

	uint32_t sreg = fs_regs[pin/10];
	uint8_t shift = (pin % 10) * 3;
	set_bfield_in_reg(sreg, 3, fun, shift);
}

void gpio_set_pin(uint8_t pin)
{
	if(pin > 57)
		return; //Error
	set_bfield_in_reg(set_regs[pin/32], 1, 1, pin % 32);
}

void gpio_clear_pin(uint8_t pin)
{
	if(pin > 57)
		return; //Error
	set_bfield_in_reg(clear_regs[pin/32], 1, 1, pin % 32);
}

void gpio_set_pull(uint8_t pin, gpio_pull pull)
{
	if(pin > 57)
		return; //Error
	set_bfield_in_reg(pull_regs[pin/16], 2, (uint32_t) pull, pin % 16);
}

int gpio_get_lvl(uint8_t pin)
{
	if(pin > 57)
		return -1; // Error
	return 0x00000001u & (mmio_read(lvl_regs[pin/32]) >> (pin % 32));
}
