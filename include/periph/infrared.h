/*
 * infrared.h - 2013年08月13日 10时54分23秒
 *
 * Copyright (c) 2013, chenchacha
 *
 */
#ifndef __INFRARED_H__
#define __INFRARED_H__

#define INFRARED_PERIPH	SYSCTL_PERIPH_GPIOB
#define INFRARED_PORT	GPIO_PORTB_BASE
#define INFRARED_PIN	GPIO_PIN_0
#define INFRARED_INT	INT_GPIOB

#define INFRARED_READ	(GPIOPinRead(INFRARED_PORT, INFRARED_PIN)&INFRARED_PIN)

extern void infrared_init(void);
extern volatile unsigned char *infrared_value(void);

#endif /* __INFRARED_H__ */
