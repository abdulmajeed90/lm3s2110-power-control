/*
 * wave.h - 2013年07月22日 15时25分22秒
 *
 * Copyright (c) 2013, chenchacha
 *
 */
#ifndef __WAVE_H__
#define __WAVE_H__

#define SPWM_TIMER_PERIPH	SYSCTL_PERIPH_TIMER0
#define SPWM_GPIO_PERIPH	SYSCTL_PERIPH_GPIOB
#define SPWM_TIMER_BASE	TIMER0_BASE
#define SPWM_GPIO_BASE	GPIO_PORTB_BASE
#define SPWM_TIEMR		TIMER_A
#define SPWM_INT_TIEMR	INT_TIMER0A
#define SPWM_PIN_1	GPIO_PIN_2
#define SPWM_PIN_2	GPIO_PIN_0
#define SPWM_MASK	(SPWM_PIN_1 | SPWM_PIN_2)

#define WAVE_INT_PPER	SYSCTL_PERIPH_GPIOD
#define WAVE_INT_PBASE	GPIO_PORTD_BASE
#define WAVE_SCAN_PIN	GPIO_PIN_6
#define WAVE_INT_PIN	GPIO_PIN_7
#define WAVE_SCAN	(GPIOPinRead(WAVE_INT_PBASE, WAVE_SCAN_PIN) & WAVE_SCAN_PIN)
#define WAVE_INT_OU GPIOPinWrite(WAVE_INT_PBASE, WAVE_INT_PIN, 0xff)
#define WAVE_INT_OD GPIOPinWrite(WAVE_INT_PBASE, WAVE_INT_PIN, 0)

#define	WAVE_32_PERIPH	SYSCTL_PERIPH_GPIOD
#define	WAVE_32_PORT	GPIO_PORTD_BASE
#define WAVE_32_PIN		GPIO_PIN_4
#define WAVE_32_INT		INT_GPIOD
#define WAVE_32_PREAD	(GPIOPinRead(WAVE_32_PORT, WAVE_32_PIN) & WAVE_32_PIN)

#define	PWM_PIN_PERIPH	SYSCTL_PERIPH_GPIOA
#define	PWM_PORT	GPIO_PORTA_BASE
#define PWM_PIN		GPIO_PIN_5
#define PWM_PREAD	(GPIOPinRead(PWM_PORT, PWM_PIN) & PWM_PIN)
#define PWM_OU		GPIOPinWrite(PWM_PORT, PWM_PIN, 0xff)
#define PWM_OD		GPIOPinWrite(PWM_PORT, PWM_PIN, 0)

extern void wave_spwm(void);
extern void wave_spwm_data(unsigned int amplitude);
extern void wave_spwm_data_step(unsigned int amplitude);
extern void wave_spwm_load(unsigned long value);

extern void wave_pwm(unsigned long period1, unsigned long period2);

extern void wave_capture(void (*capture_handler)(void));
extern void wave_interrupt_start(void);
extern void wave_interrupt_load(unsigned long value);
extern void wave_interrupt_init(unsigned long value, void (*wave_handler)(void));
extern void wave_interrupt_stop(void);
extern void wave_interrupt_clean(void);

extern void wave_cap32(void (*capture_handler)(void));
extern void wave_cap32_load(unsigned long value);
extern void wave_cap32_start(void);
extern void wave_cap32_stop(void);
extern unsigned long wave_cap32_getvalue(void);
extern void wave_cap32_clean(void);

#endif /* __WAVE_H__ */
