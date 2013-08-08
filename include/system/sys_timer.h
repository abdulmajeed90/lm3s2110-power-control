#ifndef __SYS_TIMER_H__
#define __SYS_TIMER_H__


#define  T0_A                       TIMER0_BASE , TIMER_A
#define  T0_A_CFG_16_CAP_TIME       TIMER0_BASE , TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_TIME
#define  T0_A_EVENT_POS_EDGE        TIMER0_BASE , TIMER_A , TIMER_EVENT_POS_EDGE
#define  T0_A_INT_CAP_EVENT         TIMER0_BASE , TIMER_CAPA_EVENT


#define  T1_A                   TIMER1_BASE , TIMER_A
#define  T1_B                   TIMER1_BASE , TIMER_B
#define  T1_BOTH                TIMER1_BASE , TIMER_BOTH

#define  T1_A_CFG_16_PWM     TIMER1_BASE , TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PWM
#define  T1_A_CFG_16_PER        TIMER1_BASE , TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PERIODIC
#define  T1_B_CFG_16_PER        TIMER1_BASE , TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PERIODIC
#define  T1_BOTH_CFG_16_PER        TIMER1_BASE , TIMER_CFG_16_BIT_PAIR | TIMER_CFG_B_PERIODIC | TIMER_CFG_A_PERIODIC
#define  T1_BOTH_CFG_16_PWM     TIMER1_BASE , TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM 


#define  T1_A_INT_TIMEOUT       TIMER1_BASE , TIMER_TIMA_TIMEOUT
/* #define  T1_A_INT_TIMEOUT       TIMER1_BASE , TIMER_CAPA_MATCH */
#define  T1_B_INT_TIMEOUT       TIMER1_BASE , TIMER_TIMB_TIMEOUT
#define  T1_BOTH_INT_TIMEOUT       TIMER1_BASE , TIMER_TIMB_TIMEOUT | TIMER_TIMA_TIMEOUT

typedef struct {
	unsigned long base;
	unsigned long ntimer;
	unsigned long config;
	unsigned long event_config;
	unsigned long value;
	unsigned long prescale;
	unsigned long interrupt;
	unsigned long intermod;
	void (*handler)(void);
} TIMER_t;

extern void TIMER_init_clk(void);
extern void TIMER_init_pwm(void);
extern void TIMER_init_capture(void);
extern void TIMER_init(TIMER_t *timer);

#endif /* __SYS_TIMER_H__ */
