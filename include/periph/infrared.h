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

#define MENU_0			(0x16)
#define MENU_1			(0x0c)
#define MENU_2			(0x18)
#define MENU_3			(0x5e)
#define MENU_4			(0x08)
#define MENU_5			(0x1c)
#define MENU_6			(0x5a)
#define MENU_7			(0x42)
#define MENU_8			(0x52)
#define MENU_9			(0x4a)

#define MENU_INC		(0x15)
#define MENU_DEC		(0x07)
#define MENU_FORWARD	(0x44)
#define MENU_BACK		(0x40)
#define MENU_CONFIRM	(0x43)
#define MENU_UP			(0x19)
#define MENU_DOWN		(0x0d)
#define MENU_BACKSPACE	(0x09)

extern void infrared_init(void);
extern unsigned char infrared_value(void);

#endif /* __INFRARED_H__ */
