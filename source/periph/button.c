/* effecture keyboard
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


extern void GPIO_B_routine(void);
/* button_init_gpio() - initilize GPIO for button
 */
void button_init_gpio(void)
{
	/* enable GPIO */
	SysCtlPeripheralEnable(BUTTON_PERIPH);
	/* configure GPIO in input */
	GPIOPinTypeGPIOInput(BUTTON_PORT, BUTTON_MASK);
	/* configure interrupt type */
	GPIOIntTypeSet(BUTTON_PORT, BUTTON_MASK, GPIO_FALLING_EDGE);
	/* GPIOIntTypeSet(BUTTON_PORT, BUTTON_MASK, GPIO_LOW_LEVEL); */
	/* set the interrupt prioprity */
	/* IntPrioritySet(BUTTON_INT, 0x00); */

	/* set button high to indicate entry */
/*     GPIOPinWrite(BUTTON_PORT, BUTTON_MASK, BUTTON_MASK); */

	/* register the interrupt handler */
	GPIOPortIntRegister(BUTTON_PORT, GPIO_B_routine);
	/* enable the pin interrupt */
/* 	GPIOPinIntEnable(BUTTON_PORT, BUTTON_MASK); */
	GPIOPinIntEnable(BUTTON_PORT, BUTTON_4);
	IntPrioritySet(BUTTON_INT, 1);
	/* enable the port interrupt */
	IntEnable(BUTTON_INT);
}

unsigned char button_test(unsigned char button)
{
	return GPIOPinRead(BUTTON_PORT, button);
}

/* GPIO_B_routine() - GPIO B interrupt routine
 */
volatile unsigned int gpio_b_int_status;
volatile unsigned int button_count = 0;
void GPIO_B_routine(void)
{
	unsigned int status;
	status = GPIOPinIntStatus(BUTTON_PORT, true);
	gpio_b_int_status = 1;
	GPIOPinIntClear(BUTTON_PORT, status);

	button_count++;
/*     GPIOPinWrite(BUTTON_PORT, BUTTON_MASK, BUTTON_MASK); */
}
