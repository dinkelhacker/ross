#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
#include <peripherals.h>

typedef enum {
	GPIO_F_IN 	= 0x0,
	GPIO_F_OUT	= 0x1,
	GPIO_F_ALT0	= 0x4,
	GPIO_F_ALT1	= 0x5,
	GPIO_F_ALT2	= 0x6,
	GPIO_F_ALT3	= 0x7,
	GPIO_F_ALT4	= 0x3,
	GPIO_F_ALT5	= 0x2,
} gpio_function;

typedef enum {
	GPIO_P_NONE 	= 0x0,
	GPIO_P_UP	= 0x1,
	GPIO_P_DOWN	= 0x2,
} gpio_pull;

#define REG_SIZE sizeof(uint32_t)

typedef struct gpio {
	volatile uint32_t gpfsel[((GPFSEL5 + REG_SIZE) - GPFSEL0) / sizeof(uint32_t)];
	volatile uint32_t _res0;
	volatile uint32_t gpset[((GPSET1  + REG_SIZE) - GPSET0) / sizeof(uint32_t)];
	volatile uint32_t _res1;
	volatile uint32_t gpclr[((GPCLR1 + REG_SIZE) - GPCLR0) / sizeof(uint32_t)];
	volatile uint32_t _res2;
	volatile uint32_t gplev[((GPLEV1 + REG_SIZE) - GPLEV0) / sizeof(uint32_t)];
	volatile uint32_t _res3;
	volatile uint32_t gpeds[((GPEDS1 + REG_SIZE) - GPEDS0) / sizeof(uint32_t)];
	volatile uint32_t _res4;
	volatile uint32_t gpren[((GPREN1 + REG_SIZE) - GPREN0) / sizeof(uint32_t)];
	volatile uint32_t _res5;
	volatile uint32_t gpfen[((GPFEN1 + REG_SIZE) - GPFEN0) / sizeof(uint32_t)];
	volatile uint32_t _res6;
	volatile uint32_t gphen[((GPHEN1 + REG_SIZE) - GPHEN0) / sizeof(uint32_t)];
	volatile uint32_t _res7;
	volatile uint32_t gplen[((GPLEN1 + REG_SIZE) - GPLEN0) / sizeof(uint32_t)];
	volatile uint32_t _res8;
	volatile uint32_t gparen[((GPAREN1 + REG_SIZE) - GPAREN0) / sizeof(uint32_t)];
	volatile uint32_t _res9;
	volatile uint32_t gpafen[((GPAFEN1 + REG_SIZE) - GPAFEN0) / sizeof(uint32_t)];
	volatile uint32_t _res10;
	volatile uint32_t gppull[((GPPUPPDN3 + REG_SIZE) - GPPUPPDN0) / sizeof(uint32_t)];
} gpio_t;

extern int gpio_function_select(uint8_t pin, gpio_function fun);

extern int gpio_set_pin(uint8_t pin);

extern int gpio_clear_pin(uint8_t pin);

extern int gpio_set_pull(uint8_t pin, gpio_pull pull);

extern int gpio_get_lvl(uint8_t pin);

extern int gpio_clear_event(uint8_t pin);

extern int gpio_rising_edge_detect(uint8_t pin, uint32_t status);

extern int gpio_falling_edge_detect(uint8_t pin, uint32_t status);

extern int gpio_high_level_detect(uint8_t pin, uint32_t status);

extern int gpio_low_level_detect(uint8_t pin, uint32_t status);

extern int gpio_async_rising_edge_detect(uint8_t pin, uint32_t status);

extern int gpio_async_falling_edge_detect(uint8_t pin, uint32_t status);

extern int gpio_irtrigger_high(uint8_t pin);
#endif
