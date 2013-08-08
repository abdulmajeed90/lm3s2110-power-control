#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "hw_types.h"
#include "src/gpio.h"

#define BUTTON_PERIPH		SYSCTL_PERIPH_GPIOB
#define BUTTON_PORT		GPIO_PORTB_BASE
#define BUTTON_INT		INT_GPIOB

#define BUTTON_1		GPIO_PIN_3
#define BUTTON_2		GPIO_PIN_4
#define BUTTON_3		GPIO_PIN_6
#define BUTTON_4		GPIO_PIN_5
#define BUTTON_MASK		(BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4)

extern void button_init_gpio(void);
extern unsigned char button_test(unsigned char botton);

#endif /* __BUTTON_H__ */
