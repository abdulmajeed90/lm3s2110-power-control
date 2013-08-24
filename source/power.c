/* power.c - a control for power
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "src/debug.h"
#include "src/interrupt.h"
#include "src/sysctl.h"
#include "src/timer.h"
#include "src/gpio.h"

/* module */
/* #define MODULE_SPWM */
#define MODULE_PWM
#define MODULE_LCD
#define MODULE_PLL
#define MODULE_ADS 
#define MODULE_CAP
/* #define MODULE_DAC_5618 */

#ifdef MODULE_ADS
#include "periph/ads1115.h"

#define ads_a1	1538
#define ads_a2	20270000
#define ads_current(x)	(((x)*ads_a1 - ads_a2)/10000)

#define ads_v1	1384
#define ads_v2	3055
#define ads_voltage(x)	(((x)*ads_v1 + ads_v2)/1000)
#endif

#ifdef MODULE_LCD
/* #include "periph/lcd_5110.h" */
/* #include "lcd/display.h" */
#include "lcd/menu.h"
#endif

#ifdef MODULE_DAC_5618
#include "periph/dac_5618.h"
#endif

#if defined(MODULE_SPWM) | defined(MODULE_PWM)
#include "wave.h"
#endif

#ifdef MODULE_CAP
#include "wave.h"
#include "src/pin_map.h"

#define TIMER_VALUE_DEEPIN (16)

#define TIMER_CMD_FW	0
#define TIMER_CMD_SCAN	1

volatile unsigned long timer_cap[TIMER_VALUE_DEEPIN+2];
volatile unsigned int timer_cmd = 0;
volatile unsigned int wave_pon = 0;
void timer_capture_handler(void)
{
	static unsigned char i = 0;

	timer_cap[i++] = 0xffff - TimerValueGet(TIMER0_BASE, TIMER_A);
	TimerLoadSet(TIMER0_BASE, TIMER_A, 0xffff);
	TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);

	/* get the wave form */
	i = i%TIMER_VALUE_DEEPIN;

	if ((GPIOPinRead(CCP0_PORT, CCP0_PIN) & CCP0_PIN) == 0) {
		/* follower wave pin */
		WAVE_INT_OU;
		/* Enable the follfower wave */
		wave_interrupt_start();
		/* timer interrupt to output follower wave */
		timer_cmd = TIMER_CMD_FW;
	}
}

void timer_cap32_handler(void)
{
	static unsigned char i = 0;

	wave_cap32_clean();

	wave_cap32_stop();
	timer_cap[i++] = wave_cap32_getvalue();
	wave_cap32_load(0xffffffff);
	wave_cap32_start();

	/* get the wave form */
	i = i%TIMER_VALUE_DEEPIN;

	if (WAVE_32_PREAD == 0) {
		/* follower wave pin */
		/* WAVE_INT_OU; */
		wave_interrupt_load(timer_cap[TIMER_VALUE_DEEPIN]>>1);
		/* Enable the follower wave */
		wave_interrupt_start();
		/* timer interrupt to output follower wave */
		timer_cmd = TIMER_CMD_FW;
	}
}

void timer_cap32_counter_handler(void)
{
	static unsigned char i = 0;

	wave_cap32_clean();

	wave_cap32_stop();
	timer_cap[i++] = wave_cap32_getvalue();
	wave_cap32_load(0xffffffff);
	wave_cap32_start();

	/* get the wave form */
	i = i%TIMER_VALUE_DEEPIN;
}

/* The spwm reset flag */
extern unsigned int spwm_reset;
void timer_interrupt_handler(void)
{
	wave_interrupt_clean();
	wave_interrupt_stop();

	switch (timer_cmd) {
		case TIMER_CMD_FW:
			/* Follower wave pin */
			if (wave_pon) {
				WAVE_INT_OD;
				wave_pon = 0;
				/* Spwm reset */
				spwm_reset=1;
				/* Enable spwm interrupt */
				TimerEnable(TIMER2_BASE, TIMER_A);
			}

			/* Start scan postive or nagetive */
			timer_cmd = TIMER_CMD_SCAN;
			wave_interrupt_load(timer_cap[TIMER_VALUE_DEEPIN+1]);
			wave_interrupt_start();
			break;
		case TIMER_CMD_SCAN:
			/* scan the postive or nagetive */
			wave_pon = WAVE_SCAN;
			/* follower wave pin */
			WAVE_INT_OU;
			break;
		default:
			break;
	}
}
#endif

#ifdef DEBUG
	void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

/* jtag_wait() - wait for JTAG pin. protect the JTAG
*/
#define JTAG_WAIT_PERIPH	SYSCTL_PERIPH_GPIOB
#define JTAG_WAIT_BASE		GPIO_PORTB_BASE
#define JTAG_WAIT_PIN		GPIO_PIN_0
void jtag_wait(void)
{
	/* enable key GPIO */
	SysCtlPeripheralEnable(JTAG_WAIT_PERIPH);
	/* write 1 to PIN */
	GPIOPinTypeGPIOOutput(JTAG_WAIT_BASE, JTAG_WAIT_PIN);
	GPIOPinWrite(JTAG_WAIT_BASE, JTAG_WAIT_PIN, 0xff);
	/* set key GPIO is input */
	GPIOPinTypeGPIOInput(JTAG_WAIT_BASE, JTAG_WAIT_PIN);
	/* if press button while rest */
	if (GPIOPinRead(JTAG_WAIT_BASE, JTAG_WAIT_PIN) == 0)
		for (;;);
	/* disable key GPIO */
	SysCtlPeripheralEnable(JTAG_WAIT_PERIPH);
}


int main(void)
{
	unsigned int count=0;
	/* int i=0; */

	jtag_wait();

#ifdef MODULE_PLL
	/* set LDO in 2.75v */
	SysCtlLDOSet(SYSCTL_LDO_2_75V);

	/* configure system clock
	 * PLL.
	 * main osc.
	 * xtal 8MHz.
	 * system clock div 20MHz.
	 */
	SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_8MHZ|SYSCTL_SYSDIV_8);
#endif

#ifdef MODULE_LCD
	menu_start();
#endif

#ifdef MODULE_SPWM
#include "src/pwm.h"
	wave_spwm();
#ifdef MODULE_CAP
	wave_interrupt_init(0xffff, timer_interrupt_handler);
	/* 	wave_capture(timer_capture_handler); */
	wave_cap32(timer_cap32_handler);
#endif
#endif

#ifdef MODULE_PWM
	wave_pwm(8000, 1);
#ifdef MODULE_CAP
	wave_interrupt_init(0xffff, timer_interrupt_handler);
	/* 	wave_capture(timer_capture_handler); */
	wave_cap32(timer_cap32_counter_handler);
#endif
#endif



#ifdef MODULE_ADS
	ads_init();
#endif

	/* enable systerm interrupt */
	IntMasterEnable();

#ifdef MODULE_DAC_5618
	DAC_init_gpio();
	DAC_write_data(0x1ff, 0);
#endif


	/* initialize menu data */
#ifdef MODULE_LCD
#ifdef MODULE_ADS
	MENU_PARAMETER_t menu_para;
	menu_init_parameter(2, &menu_para);
#endif
#ifdef MODULE_CAP
	MENU_WAVE_t menu_wave;

	menu_wave.frequency = 1000;
	menu_init_wave(1, &menu_wave);
#endif
#endif

	while(1) {
#ifdef MODULE_DAC_5618
		unsigned short int i;

		DAC_write_data(i++, 0);
		DAC_write_data(i++, 1);
#endif

#ifdef MODULE_LCD


#ifdef MODULE_ADS
		unsigned int ads_value;

		/* ADS channel 2 */
		for (count = 5; count; count--)
			ads_value = ads_read(1);

		menu_para.voltage = ads_voltage(ads_value);

		/* ADS channel 1 */
		for (count = 5; count; count--)
			ads_value = ads_read(2);

		menu_para.current = ads_current(ads_value);

		/* ADS channel 1 */
		/* 		for (count = 10; count; count--)
		 * 			ads_value = ads_read(3);
		 * 
		 * 		sprintf(string, "ADC4: %6d", ads_value);
		 * 		menu_add_string(i++, string);
		 */
#endif

#ifdef MODULE_CAP
		unsigned long tmp1, tmp2;
		/* Get the capture average */
		for (tmp1 = timer_cap[0], count=2; count<TIMER_VALUE_DEEPIN; count+=2)
			tmp1 = (tmp1 + timer_cap[count]) >> 1;

		for (tmp2 = timer_cap[1], count=3; count<TIMER_VALUE_DEEPIN; count+=2)
			tmp2 = (tmp2 + timer_cap[count]) >> 1;

		/* load value to follower wave */
		menu_wave.period1 = timer_cap[TIMER_VALUE_DEEPIN] = (tmp1<tmp2?tmp1:tmp2);
		menu_wave.period2 = timer_cap[TIMER_VALUE_DEEPIN+1] = (tmp1>tmp2?tmp1:tmp2);

#ifdef MODULE_SPWM
		/* Sync spwm period with input */
		/* 		wave_spwm_load((tmp1+tmp2)/85); */
		/* 		wave_spwm_load((tmp1+tmp2)/42); */
#endif
#endif

		/* Display the screen */
		menu_display();
#endif
	}

#ifdef MODULE_LCD
	menu_end();
#endif

	return 0;
}
