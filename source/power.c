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
/* #define MODULE_SPWM
 */
/* #define MODULE_SPWM_DOUBLE
 */
/* #define MODULE_PWM
 */
/* #define MODULE_LCD
 */
#define MODULE_PLL
/* #define MODULE_ADS
 */
/* #define MODULE_CAP
 */
/* #define MODULE_DAC_5618 */
/* #define MODULE_INA209
 */

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

#if defined(MODULE_SPWM) | defined(MODULE_PWM) | defined(MODULE_SPWM_DOUBLE)
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
#if 0
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
#endif

void timer_cap32_handler(void) {
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
		WAVE_INT_OU;
		wave_interrupt_load((timer_cap[TIMER_VALUE_DEEPIN]>>1));
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
			wave_interrupt_load(timer_cap[TIMER_VALUE_DEEPIN+1]/2);
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
	unsigned int i=0, j=0;
	unsigned int temp_spwm_counter_step=60;
	int pflag=0, nflag=0;
/* 	unsigned int wave_form=0;
 * 	unsigned int wave_form_out=1;
 * 	unsigned int wave_np_count=0;
 * 	unsigned int wave_np[10];
 * 	unsigned int tmp=0;
 * 	unsigned int wave_delay_n=0;
 * 	unsigned int wave_delay_p=0;
 */

/* 	unsigned int wave_advance_n=0;
 * 	unsigned int wave_advance_p=0;
 */

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
	SysCtlClockSet(SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_6MHZ|SYSCTL_SYSDIV_2);
#endif

#ifdef MODULE_LCD
	menu_start();
#endif

#if defined(MODULE_SPWM) | defined(MODULE_SPWM_DOUBLE)
#ifdef MODULE_CAP
	wave_interrupt_init(0xffff, timer_interrupt_handler);
	/* 	wave_capture(timer_capture_handler); */
	wave_cap32(timer_cap32_handler);
#endif

#ifdef MODULE_SPWM
#include "src/pwm.h"
	wave_spwm();
#elif defined(MODULE_SPWM_DOUBLE)
	wave_spwm_double_init(0);
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
#ifdef MODULE_INA209
	MENU_INA_t menu_ina;

	menu_init_ina(1, &menu_ina);
#endif
#ifdef MODULE_SPWM_DOUBLE
	MENU_SPWM_DOUBLE_t menu_spwm;

	menu_init_spwm_double(3, &menu_spwm);
#endif
#endif

#if 1
#include <string.h>
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "src/sysctl.h"
#include "src/debug.h"
#include "src/interrupt.h"
#include "src/sysctl.h"
#include "src/timer.h"
#include "src/gpio.h"
#include "src/pwm.h"
#include "src/pin_map.h"
#include "wave.h"


	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(WAVE_INT_PPER);

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOInput(WAVE_INT_PBASE, WAVE_SCAN_PIN);
#endif

	while(1) {
#ifdef MODULE_DAC_5618
		unsigned long int i = 0;

		DAC_write_data(i++, 0);
		DAC_write_data(i++, 1);
#endif



#ifdef MODULE_ADS
		unsigned int ads_value;

		/* ADS channel 2 */
		for (count = 5; count; count--)
			ads_value = ads_read(1);

#ifdef MODULE_LCD
		menu_para.voltage = ads_voltage(ads_value);
#endif

		/* ADS channel 1 */
		for (count = 5; count; count--)
			ads_value = ads_read(2);

#ifdef MODULE_LCD
		menu_para.current = ads_current(ads_value);
#endif

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

		/* the top array is period */
		timer_cap[TIMER_VALUE_DEEPIN] = (tmp1<tmp2?tmp1:tmp2);
		timer_cap[TIMER_VALUE_DEEPIN+1] = (tmp1>tmp2?tmp1:tmp2);


#if defined(MODULE_SPWM) | defined(MODULE_SPWM_DOUBLE)
		/* Sync spwm period with input */
/* 		wave_spwm_load((tmp1+tmp2)/85);
 * 		wave_spwm_load((tmp1+tmp2)/42);
 */
/* 		wave_spwm_load((tmp1+tmp2)/64);
 */
/* 		wave_spwm_load((timer_cap[TIMER_VALUE_DEEPIN]+
 * 					timer_cap[TIMER_VALUE_DEEPIN+1])/32);
 */
/* 		wave_spwm_load((tmp1+tmp2)/256);
 */

#endif
#endif

#ifdef MODULE_LCD
extern unsigned int temp_spwm_counter;
		/* load value to follower wave */
		menu_wave.period1 = timer_cap[TIMER_VALUE_DEEPIN];
		menu_wave.period2 = timer_cap[TIMER_VALUE_DEEPIN+1];

		/* Display the screen */
		menu_display();
#endif

#if 1
#define SPWM_STEP	32
#include "wave.h"
#include "src/pin_map.h"

		
#if 0
		/* Get wave form */
#if 0
		if (WAVE_SCAN) {
			if (wave_delay_p++ >= temp_spwm_counter_step*10) {
				wave_form=1;
			}
			wave_delay_n = 0;
		} else {
			if (wave_delay_n++ >= temp_spwm_counter_step*10) {
				wave_form=0;
			}
			wave_delay_p = 0;
		}
#endif

		/* Get the level edge */
		wave_np[wave_np_count++%5] = WAVE_SCAN;
		for (count=0, tmp=0; count < 5; count++) {
			tmp += wave_np[count];
		}
		if (tmp/3) {
			wave_form = 1;
		} else {
			wave_form = 0;
		}

#define WAVE_DELAY	0

		/* Delay the wave output */
		if (wave_form_out != wave_form) {
			/* Positve */
			if (wave_form == 1) {
#if 1
				/* Positive delay */
				if (wave_delay_p++ >= WAVE_DELAY) {
					wave_form_out = 1;
					wave_delay_p = 0;
				}
#endif
			} else { /* Negative */
#if 1
				/* Negative delay */
				if (wave_delay_n++ >= WAVE_DELAY) {
					wave_form_out = 0;
					wave_delay_n = 0;
				}
#endif
			}
		}

#endif

#define COUNT_DELAY 60
/* 		if (wave_form_out) {
 */
		if (WAVE_SCAN) {
			/* if new period */
			pflag = 1;
			if (nflag == 1) {
				temp_spwm_counter_step=i/SPWM_STEP*2;
				nflag = 0;
				i=0;
			}
			i++;
			/* Configure switch level
			 * P0 low and P1 high.
			 */
			switch (i%SPWM_STEP) {
				/* P0 P1 high */
				/* P0 and P1 low(...) */
				case 0: /* 1,1 */
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
						break;
				case 2: 
				case 4: /* 3,2 */
/* 					if (i>2*temp_spwm_counter_step &&
 * 							i<(SPWM_STEP/2-2)*temp_spwm_counter_step) {
 * 						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
 * 					}
 * 					break;
 */
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
						break;
				case 6:
				case 8:
					if (i>2*temp_spwm_counter_step &&
							i<(SPWM_STEP/2-2)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;

				case 10: /* 6,3 */
					if (i>2*temp_spwm_counter_step &&
							i<(SPWM_STEP/2-2)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 12:
				case 14:
				case 16: /* 9,4 */
					if (i>3*temp_spwm_counter_step &&
							i<(SPWM_STEP/2-3)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 18:
				case 20: /* 11,5 */
					if (i>5*temp_spwm_counter_step &&
							i<(SPWM_STEP/2-5)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 22:
				case 24: /* 13,6 */
					if (i>(5*temp_spwm_counter_step) &&
							i<(SPWM_STEP/2-5)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 26:
				case 28: /* 15,7 */
					if (i>(7*temp_spwm_counter_step) &&
							i<(SPWM_STEP/2-7)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 30: /* 16, 8 */
					if (i>(8*temp_spwm_counter_step) &&
							i<(SPWM_STEP/2-8)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;

					/* free */
				default:
/* 					for (count=COUNT_DELAY; count; count--);
 */
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_1);
					break;
			}
			if (i%SPWM_STEP%2==0) {
				if (i%SPWM_STEP < 8)
					for (count=1*COUNT_DELAY; count; count--);
				else
					for (count=2*COUNT_DELAY; count; count--);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_1);
			}
		} else {	/* Negative */
			nflag=1;
			/* if negative period */
			if (pflag == 1) {
				temp_spwm_counter_step=j/SPWM_STEP*2;
				pflag = 0;
				j=0;
			}
			j++;
			/* Configure switch level
			 * P0 high and P1 low.
			 */
			switch (j%SPWM_STEP) {
				case 0: /* 1,1 */
				case 2: 
				case 4: /* 3,2 */
/* 					if (j>2*temp_spwm_counter_step &&
 * 							j<(SPWM_STEP/2-2)*temp_spwm_counter_step) {
 * 						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
 * 					}
 * 					break;
 */
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
						break;
				case 6:
				case 8:
					if (j>2*temp_spwm_counter_step &&
							j<(SPWM_STEP/2-2)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;

				case 10: /* 6,3 */
					if (j>2*temp_spwm_counter_step &&
							j<(SPWM_STEP/2-2)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 12:
				case 14:
				case 16: /* 9,4 */
					if (j>3*temp_spwm_counter_step &&
							j<(SPWM_STEP/2-3)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 18:
				case 20: /* 11,5 */
					if (j>5*temp_spwm_counter_step &&
							j<(SPWM_STEP/2-5)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 22:
				case 24: /* 13,6 */
					if (j>(5*temp_spwm_counter_step) &&
							j<(SPWM_STEP/2-5)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 26:
				case 28: /* 15,7 */
					if (j>(7*temp_spwm_counter_step) &&
							j<(SPWM_STEP/2-7)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;
				case 30: /* 16, 8 */
					if (j>(8*temp_spwm_counter_step) &&
							j<(SPWM_STEP/2-8)*temp_spwm_counter_step) {
						GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
					}
					break;

					/* free */
				default:
/* 					for (count=COUNT_DELAY; count; count--);
 */
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0);
					break;
			}
			if (j%SPWM_STEP%2 == 0) {
				if (i%SPWM_STEP < 8)
					for (count=1*COUNT_DELAY; count; count--);
				else
					for (count=2*COUNT_DELAY; count; count--);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_0);
			}
		}
		/* Delay */
/* 		for (count=2; count; count--);
 */
#endif
	}

#ifdef MODULE_LCD
	menu_end();
#endif

	return 0;
}
