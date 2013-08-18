/*
 * delay.h - 2013年08月13日 11时14分08秒
 *
 * Copyright (c) 2013, chenchacha
 *
 * Delay exactly macros.
 */
#ifndef __DELAY_H__
#define __DELAY_H__

#include "hw_types.h"
#include "src/sysctl.h"

#define delay_ms(a)	SysCtlDelay((a) * (SysCtlClockGet() / 3000));
#define delay_us(a)	SysCtlDelay((a) * (SysCtlClockGet() / 3000000));

#endif /* __DELAY_H__ */
