#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#define PERIPHERAL_BASE 0xFE000000

// GPIO 
enum {
	GPIO_BASE 	= PERIPHERAL_BASE + 0x200000,
	GPFSEL0         = GPIO_BASE + 0x00, 
	GPFSEL1         = GPIO_BASE + 0x04, 
	GPFSEL2         = GPIO_BASE + 0x08, 
	GPFSEL3         = GPIO_BASE + 0x0c, 
	GPFSEL4         = GPIO_BASE + 0x10, 
	GPFSEL5         = GPIO_BASE + 0x14, 
	GPSET0          = GPIO_BASE + 0x1c,
	GPSET1          = GPIO_BASE + 0x20,
	GPCLR0          = GPIO_BASE + 0x28,
	GPCLR1          = GPIO_BASE + 0x2c,
	GPLEV0		= GPIO_BASE + 0x34,
	GPLEV1		= GPIO_BASE + 0x38,
	GPEDS0		= GPIO_BASE + 0x40,
	GPEDS1		= GPIO_BASE + 0x44,
	GPREN0		= GPIO_BASE + 0x4c,
	GPREN1		= GPIO_BASE + 0x50,
	GPFEN0		= GPIO_BASE + 0x58,
	GPFEN1		= GPIO_BASE + 0x5c,
	GPHEN0		= GPIO_BASE + 0x64,
	GPHEN1		= GPIO_BASE + 0x68,
	GPLEN0		= GPIO_BASE + 0x70,
	GPLEN1		= GPIO_BASE + 0x74,
	GPAREN0		= GPIO_BASE + 0x7c,
	GPAREN1		= GPIO_BASE + 0x80,
	GPAFEN0		= GPIO_BASE + 0x88,
	GPAFEN1		= GPIO_BASE + 0x8c,
	GPPUPPDN0       = GPIO_BASE + 0xe4,
	GPPUPPDN1       = GPIO_BASE + 0xe8,
	GPPUPPDN2       = GPIO_BASE + 0xec,
	GPPUPPDN3       = GPIO_BASE + 0xf0,
};

enum {
	GPIO_MAX_PIN       = 53,
	GPIO_FUNCTION_ALT5 = 2,
};

enum {
	Pull_None = 0,
};

// UART
enum {
	AUX_BASE        = PERIPHERAL_BASE + 0x215000,
	AUX_ENABLES     = 4,
	AUX_MU_IO_REG   = 64,
	AUX_MU_IER_REG  = 68,
	AUX_MU_IIR_REG  = 72,
	AUX_MU_LCR_REG  = 76,
	AUX_MU_MCR_REG  = 80,
	AUX_MU_LSR_REG  = 84,
	AUX_MU_CNTL_REG = 96,
	AUX_MU_BAUD_REG = 104,
	AUX_UART_CLOCK  = 500000000,
	UART_MAX_QUEUE  = 16 * 1024
};

// SYSTEM TIMER
enum {
	SYS_TIMER_BASE  = PERIPHERAL_BASE + 0x3000,
	SYS_TIMER_CS    = SYS_TIMER_BASE  + 0x00,
	SYS_TIMER_CLO   = SYS_TIMER_BASE  + 0x04,
	SYS_TIMER_CHI   = SYS_TIMER_BASE  + 0x08,
	SYS_TIMER_C0    = SYS_TIMER_BASE  + 0x0c,
	SYS_TIMER_C1    = SYS_TIMER_BASE  + 0x10,
	SYS_TIMER_C2    = SYS_TIMER_BASE  + 0x14,
	SYS_TIMER_C3    = SYS_TIMER_BASE  + 0x18,
};

// PM
enum {
	PM_BASE 	= PERIPHERAL_BASE + 0x100000,
	PM_RSTC 	= PM_BASE + 0x1c,
	PM_WDOG 	= PM_BASE + 0x24,
	PM_PASSWORD 	= 0x5a000000,
	PM_RSTC_RESET 	= 0x00000020,
};

enum {
	GIC400_BASE	= 0xFF840000UL,
};


#endif
