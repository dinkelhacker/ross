#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

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
#endif
