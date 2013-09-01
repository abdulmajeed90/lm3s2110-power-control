/*
 * wave.c - 2013年07月22日 15时21分44秒
 *
 * Copyright (c) 2013, chenchacha
 * 
 */
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
#include "system/sys_timer.h"
#include "system/sys_pwm.h"
#include "data/spwm.h"

unsigned char spwm_a[1024];
unsigned char spwm_b[1024];
/* unsigned char spwm_step=12; */
/* unsigned char spwm_step=24; */
unsigned char spwm_step=50;
unsigned char spwm_value=0xf;
unsigned char spwm_flag=0;
unsigned int spwm_count_u=10;
unsigned int spwm_count_d=10;
void pwm_spwm_handler(void)
{

	PWMGenIntClear(PWM_BASE, PWM_GEN_0, PWM_INT_GEN_0);

	/* Output the switch wave */
	if (spwm_flag) {
			PWM_OD;
	} else {
			PWM_OU;
	}

	/* 	IntDisable(INT_PWM0);
	*/
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, spwm_value);

}
unsigned int spwm_reset=0;
void time_spwm_handler(void)
{
	static unsigned int wave_flag=1;
	static unsigned int i=0, j=0;

	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	if (spwm_reset == 1) {
		i=j=0;
		spwm_reset=0;
		wave_flag=1;
	}

	if (wave_flag) {
		spwm_value=spwm_a[i]+0x1;
		i+=spwm_step;
		spwm_flag=1;
	} else {
		spwm_value=spwm_b[j]+0x1;
		j+=spwm_step;
		spwm_flag=0;
	}

	if (i > 1024) {
		i = 0;
		/* Enable pwm interrupt for switch wave */
		/* 		IntEnable(INT_PWM0); */
		/* Output negative period */
		wave_flag=0;
	}
	if (j > 1024) {
		j=0;
		/* Enable pwm interrupt for switch wave */
		/* 		IntEnable(INT_PWM0); */
		/* Output postive period */
		wave_flag=1;
		/* Close timer */
		TimerDisable(TIMER2_BASE, TIMER_A);
	}

}

/* wave_spwm_data - initialize the sin data with amplitude
*/
void wave_spwm_data(unsigned int amplitude)
{
	int n;

	for (n=0; n < 512; n++) {
		spwm_a[n] = spwm_data[n]/amplitude;
		spwm_a[1023-n] = spwm_data[n]/amplitude;
	}
	for (n=0; n < 512; n++) {
		spwm_b[n] = 0xff - (spwm_data[n]/amplitude);
		spwm_b[1023-n] = 0xff - (spwm_data[n]/amplitude);
	}
}		/* -----  end of function wave_spwm_data  ----- */

/* wave_spwm_data_step - initialize the sin data with amplitude with step
 * The wave step is 64.
 */
void wave_spwm_data_step(unsigned int amplitude)
{
	int n;
	unsigned long tmp;

	for (n=0; n < 512; n++) {
		tmp = ((unsigned long)spwm_data[n] * amplitude) >> 6;
		spwm_a[n] = (unsigned char)tmp;
		spwm_a[1023-n] = (unsigned char)tmp;
	}
	for (n=0; n < 512; n++) {
		tmp = 0xff - (((unsigned long)spwm_data[n] * amplitude) >> 6);
		spwm_b[n] = (unsigned char)tmp;
		spwm_b[1023-n] = (unsigned char)tmp;
	}

}		/* -----  end of function wave_spwm_data_step  ----- */

/* wave_spwm - output a spwm
 */
void wave_spwm(void)
{
	PWM_t pwm1;

	/* sin wave data */
	wave_spwm_data(1);

	/* Configure for GPIO */
    SysCtlPeripheralEnable(PWM_PIN_PERIPH);
	GPIOPinTypeGPIOOutput(PWM_PORT, PWM_PIN);

	/* Configure pwm counter */
	pwm1.gpio_periph = PWM0_PERIPH;
	pwm1.gpio_base = PWM0_PORT;
	pwm1.gpio = PWM0_PIN;
	pwm1.gen = PWM_GEN_0;
	pwm1.config = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC;
	pwm1.period = 0xff;
	pwm1.width = pwm1.period / 2;
	pwm1.out = PWM_OUT_0;
	pwm1.outbit = PWM_OUT_0_BIT;
	/* Configure for pwm interrupt */
#if 1
	pwm1.trig = PWM_INT_CNT_ZERO;
	pwm1.intergen = PWM_INT_GEN_0;
	pwm1.handler = pwm_spwm_handler;
	pwm1.interrupt = INT_PWM0;
	PWM_init(&pwm1);
/* 	IntEnable(INT_PWM0);
 */
	/* Configure the periority of interrupt */
	IntPrioritySet(INT_PWM0, 3);
#endif

#if 1
	/* Enable the peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	TIMER_t timer;
	timer.base = TIMER2_BASE;
	timer.ntimer = TIMER_A;
	timer.config = TIMER_CFG_32_BIT_PER;
/* 	timer.config = TIMER_CFG_A_CAP_TIME | TIMER_CFG_16_BIT_PAIR;
 */
	/* 50Hz */
	timer.value = 5800;
	timer.interrupt = INT_TIMER2A;
	timer.prescale = 0;
	timer.intermod = TIMER_TIMA_TIMEOUT;
	timer.handler = time_spwm_handler;
	TIMER_init(&timer);
	/* Configure the periority of interrupt */
	IntPrioritySet(INT_TIMER2A, 0);

	TimerEnable(timer.base, timer.ntimer);
#endif
}

/* wave_spwm_load - load the timer interrupt value
 */
void wave_spwm_load(unsigned long value)
{
	TimerLoadSet(TIMER2_BASE, TIMER_A, value);
}		/* -----  end of function wave_spwm_load  ----- */

/* wave_pwm - output two pwm
*/
void wave_pwm(unsigned long period1, unsigned long period2)
{
	PWM_t pwm1;
	int count = SysCtlClockGet();

	/* Configure pwm counter */
	pwm1.gpio_periph = PWM0_PERIPH;
	pwm1.gpio_base = PWM0_PORT;
	pwm1.gpio = PWM0_PIN;
	pwm1.gen = PWM_GEN_0;
/* 	pwm1.config = PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC;
 */
	/* configure pwm1 */
	pwm1.config = PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC;
	pwm1.period = count / period1;
	pwm1.width = pwm1.period / 3;
	pwm1.out = PWM_OUT_0;
	pwm1.outbit = PWM_OUT_0_BIT;
	pwm1.handler = 0;
	PWM_init(&pwm1);

#if 0
	pwm1.gpio_periph = SYSCTL_PERIPH_GPIOD;
	pwm1.gpio_base = GPIO_PORTD_BASE;
	pwm1.gpio = GPIO_PIN_1;
	pwm1.gen = PWM_GEN_1;
	pwm1.config = PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC;
	pwm1.period = count / period2;
	pwm1.width = pwm1.period / 2;
	pwm1.out = PWM_OUT_1;
	pwm1.outbit = PWM_OUT_1_BIT;
	pwm1.handler = 0;
	PWM_init(&pwm1);

	/* configure pwm2 */
	pwm2.gpio_periph = SYSCTL_PERIPH_GPIOB;
	pwm2.gpio_base = GPIO_PORTB_BASE;
	pwm2.gpio = GPIO_PIN_0;
	pwm2.gen = PWM_GEN_2;
	pwm2.config = PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC;
	pwm2.period = count / period2;
	pwm2.width = pwm2.period / 2;
	pwm2.out = PWM_OUT_2;
	pwm2.outbit = PWM_OUT_2_BIT;
	pwm2.handler = 0;
 	PWM_init(&pwm2);
#endif
}		/* -----  end of function wave_pwm  ----- */

/* wave_pwm_value - pwm load period value
 */
void wave_pwm_value(unsigned long value)
{
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, value);
    PWMPulseWidthSet(PWM_BASE, PWM_GEN_0, value>>1);
}		/* -----  end of function wave_pwm_value  ----- */

/* wave_pwm_get_value -
*/
unsigned long wave_pwm_get_value(void)
{
	return PWMGenPeriodGet(PWM_BASE, PWM_GEN_0);
}		/* -----  end of function wave_pwm_get_value  ----- */

/* wave_capture - wave capture by timer
 * @capture_handler: the handler function for capture.
 * enable timer0 channel A and CCP0(PA0).
 * capture all of edges. both positive edges and negative edges.
 */
void wave_capture(void (*capture_handler)(void))
{
	TIMER_t timer;

	/* Enable the peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(CCP0_PERIPH);
	/* Set GPIO B0 as an output */
	GPIOPinTypeTimer(CCP0_PORT, CCP0_PIN);

	/* Configure timer structure */
	timer.base = TIMER0_BASE;
	timer.ntimer = TIMER_A;
	/* Timer A event timer and Two 16-bit timers */
	timer.config = TIMER_CFG_A_CAP_TIME | TIMER_CFG_16_BIT_PAIR;
	timer.event_config = TIMER_EVENT_BOTH_EDGES;
	timer.value = 0xffff;
	timer.interrupt = INT_TIMER0A;
	timer.prescale = 0;
	/* CaptureA event interrupt */
	timer.intermod = TIMER_CAPA_EVENT;
	timer.handler = capture_handler;
	TIMER_init(&timer);

	TimerEnable(timer.base, timer.ntimer);
}		/* -----  end of function wave_capture  ----- */

/* wave_capture_32 - wave capture by timer
 * @capture_handler: the handler function for capture.
 * enable timer0 channel A and CCP0(PA0).
 * capture all of edges. both positive edges and negative edges.
 */
void wave_cap32(void (*capture_handler)(void))
{
	TIMER_t timer;

	/* Enable the peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(WAVE_32_PERIPH);
	/* Configure GPIO in input */
	GPIOPinTypeGPIOInput(WAVE_32_PORT, WAVE_32_PIN);
	/* Configure interrupt type */
	GPIOIntTypeSet(WAVE_32_PORT, WAVE_32_PIN, GPIO_BOTH_EDGES);

	/* Register the interrupt handler */
	GPIOPortIntRegister(WAVE_32_PORT, capture_handler);
	/* Enable the pin interrupt */
	GPIOPinIntEnable(WAVE_32_PORT, WAVE_32_PIN);

	/* Configure timer structure */
	timer.base = TIMER0_BASE;
	timer.ntimer = TIMER_A;
	timer.config = TIMER_CFG_32_BIT_PER;
	timer.value = 0xffffffff;
	timer.event_config = 0xffffffff;
	timer.prescale = 0;
	timer.handler = 0;
	TIMER_init(&timer);

	/* Enable the port interrupt */
	TimerEnable(timer.base, timer.ntimer);
	IntEnable(WAVE_32_INT);
}		/* -----  end of function wave_capture  ----- */

/* wave_cap32_load - load the timer interrupt value
 */
void wave_cap32_load(unsigned long value)
{
	TimerLoadSet(TIMER0_BASE, TIMER_A, value);
}		/* -----  end of function wave_cap32_load  ----- */

/* wave_cap32_start - start counter
 */
void wave_cap32_start(void)
{
	TimerEnable(TIMER0_BASE, TIMER_A);
}		/* -----  end of function wave_cap32_start  ----- */

/* wave_cap32_stop -
 */
void wave_cap32_stop(void)
{
	TimerDisable(TIMER0_BASE, TIMER_A);
}		/* -----  end of function wave_cap32_stop  ----- */

/* wave_cap32_getvalue -
 */
unsigned long wave_cap32_getvalue(void)
{
	return (0xffffffff - TimerValueGet(TIMER0_BASE, TIMER_A));
}		/* -----  end of function wave_cap32_getvalue  ----- */

/* wave_cap32_clean -
 */
void wave_cap32_clean(void)
{
	unsigned int status;

	status = GPIOPinIntStatus(WAVE_32_PORT, true);
	GPIOPinIntClear(WAVE_32_PORT, status);
}		/* -----  end of function wave_cap32_clean  ----- */

/* wave_interrupt_init - enable a timer interrupt
 * @value: the count value.
 * @wave_handler: the handler function for capture.
 */
void wave_interrupt_init(unsigned long value, void (*wave_handler)(void))
{
	/* Enable the peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

	TIMER_t timer;
	timer.base = TIMER1_BASE;
	timer.ntimer = TIMER_A;
	timer.config = TIMER_CFG_32_BIT_PER;
	timer.value = value;
	timer.interrupt = INT_TIMER1A;
	timer.prescale = 0;
	timer.intermod = TIMER_TIMA_TIMEOUT;
	timer.handler = wave_handler;
	TIMER_init(&timer);

	/* Eable output pin */
	SysCtlPeripheralEnable(WAVE_INT_PPER);
	GPIOPinTypeGPIOOutput(WAVE_INT_PBASE, WAVE_INT_PIN);
	GPIOPinTypeGPIOInput(WAVE_INT_PBASE, WAVE_SCAN_PIN);

	GPIOPinWrite(GPIO_PORTD_BASE, WAVE_INT_PIN, 0);
}		/* -----  end of function wave_interrupt_init  ----- */

/* wave_interrupt_load - load the timer interrupt value
 */
void wave_interrupt_load(unsigned long value)
{
	TimerLoadSet(TIMER1_BASE, TIMER_A, value);
}		/* -----  end of function wave_interrupt_load  ----- */

/* wave_interrupt_start - start counter
 */
void wave_interrupt_start(void)
{
	TimerEnable(TIMER1_BASE, TIMER_A);
}		/* -----  end of function wave_interrupt_start  ----- */

/* wave_interrupt_stop -
 */
void wave_interrupt_stop(void)
{
	TimerDisable(TIMER1_BASE, TIMER_A);
}		/* -----  end of function wave_interrupt_stop  ----- */

/* wave_interrupt_clean -
 */
void wave_interrupt_clean(void)
{
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}		/* -----  end of function wave_interrupt_clean  ----- */
