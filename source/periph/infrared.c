/*
 * infrared.c - 2013年08月13日 10时53分13秒
 *
 * Copyright (c) 2013, chenchacha
 *
 * The infrared remote cotrol. Use a GPIO replace keyboard. 
 */
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "src/debug.h"
#include "src/sysctl.h"
#include "src/interrupt.h"
#include "src/gpio.h"
#include "periph/button.h"
#include "src/pin_map.h"

#include "periph/infrared.h"
#include "delay.h"


static int infrared_decode(volatile unsigned char *dat);
volatile unsigned char infrared_data[4];
volatile unsigned int infrared_flag=0;
/* infrared_handler - the infrared interrupt handler
 */
void infrared_handler(void)
{
	unsigned int status;
	status = GPIOPinIntStatus(INFRARED_PORT, true);
	GPIOPinIntClear(INFRARED_PORT, status);

	IntDisable(INFRARED_INT);

	if (infrared_decode(infrared_data) == true)
		infrared_flag = 1;

	IntEnable(INFRARED_INT);
}		/* -----  end of function infrared_handler  ----- */

/* infrared_init - initialize infrared
 * configure input GPIO and interrupt.
 */
void infrared_init(void)
{
	/* Enable GPIO */
	SysCtlPeripheralEnable(INFRARED_PERIPH);
	/* Configure GPIO in input */
	GPIOPinTypeGPIOInput(INFRARED_PORT, INFRARED_PIN);
	/* Configure interrupt type */
	GPIOIntTypeSet(INFRARED_PORT, INFRARED_PIN, GPIO_FALLING_EDGE);
	/* Register interrupt handler */
	GPIOPortIntRegister(INFRARED_PORT, infrared_handler);
	/* Enable infrared interrupt */
	GPIOPinIntEnable(INFRARED_PORT, INFRARED_PIN);
	/* Configure the interrupt priority in a low degree */
	IntPrioritySet(INFRARED_INT, 0);
	/* Enable GPIO interrupt */
	IntEnable(INFRARED_INT);
}		/* -----  end of function infrared_init  ----- */

/* infrared_decode - decode the infrared code
 */
int infrared_decode(volatile unsigned char *dat)
{
	int i;
	int m, n;
#if 0
	delay_ms(10);
	/* Validate shoot */
	if (INFRARED_READ == 1)
		return false;
#endif

	/* Jump over 9ms shoot */
	while (INFRARED_READ == 0) {
		delay_ms(1);
		i++;
		if (i>20)
			return false;
	}

	/* Jump over 4.5ms blank */
	for (i=0; INFRARED_READ == 1; i++) {
		delay_ms(1);
		if (i>10)
			return false;
	}

	/* The data is 4 group with 8 bit */
	for (m=0; m<4; m++) {
		for (n=0; n<8; n++) {
			/* Jump over shoot */
			for (i=0; INFRARED_READ == 1; i++) {
				delay_ms(1);
				if (i > 10)
					return false;
			}
			/* count blank */
			for (i=0; INFRARED_READ == 0; i++) {
				delay_ms(1);
				if (i > 3)
					return false;
			}

			if (i >= 2)
				dat[m] = (dat[m] >> 1) | 0x80;
			else
				dat[m] = (dat[m] >> 1);
		}
	}
	return true;
}

/* infrared_value -
*/
volatile unsigned char *infrared_value(void)
{
	return infrared_data;
}		/* -----  end of function infrared_value  ----- */
