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

#define INF_MENU_0			(0x16)
#define INF_MENU_1			(0x0c)
#define INF_MENU_2			(0x18)
#define INF_MENU_3			(0x5e)
#define INF_MENU_4			(0x08)
#define INF_MENU_5			(0x1c)
#define INF_MENU_6			(0x5a)
#define INF_MENU_7			(0x42)
#define INF_MENU_8			(0x52)
#define INF_MENU_9			(0x4a)

#define INF_MENU_INC		(0x07)
#define INF_MENU_DEC		(0x15)
#define INF_MENU_FORWARD	(0x44)
#define INF_MENU_BACK		(0x40)
#define INF_MENU_CONFIRM	(0x43)

extern void infrared_init(void);
extern volatile unsigned char *infrared_value(void);

#endif /* __INFRARED_H__ */
