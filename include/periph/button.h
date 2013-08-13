#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "hw_types.h"
#include "src/gpio.h"

#define BUTTON_PERIPH		SYSCTL_PERIPH_GPIOF
#define BUTTON_PORT		GPIO_PORTF_BASE
#define BUTTON_INT		INT_GPIOF

#define BUTTON_1		GPIO_PIN_0
#define BUTTON_2		GPIO_PIN_1
#define BUTTON_3		GPIO_PIN_6
#define BUTTON_4		GPIO_PIN_5
#define BUTTON_MASK		(BUTTON_2)

extern void button_init_gpio(void);
extern unsigned char button_test(unsigned char botton);

#endif /* __BUTTON_H__ */
