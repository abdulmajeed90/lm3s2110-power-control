#ifndef __SYS_PWM_H__
#define __SYS_PWM_H__

typedef struct { 
	unsigned long gpio_base;
	unsigned long gpio_periph;
	unsigned long gpio;
	unsigned long gen; /*	PWM_GEN_0
							PWM_GEN_1
							PWM_GEN_2
							PWM_GEN_3	*/
	unsigned long config; /* PWM_GEN_MODE_DOWN
							 PWM_GEN_MODE_UP_DOWN
							 PWM_GEN_MODE_SYNC
							 PWM_GEN_MODE_NO_SYNC
							 PWM_GEN_MODE_DBG_RUN
							 PWM_GEN_MODE_DBG_STOP
							 PWM_GEN_MODE_FAULT_LATCHED
							 PWM_GEN_MODE_FAULT_UNLATCHED
							 PWM_GEN_MODE_FAULT_MINPER
							 PWM_GEN_MODE_FAULT_NO_MINPER
							 PWM_GEN_MODE_FAULT_EXT
							 PWM_GEN_MODE_FAULT_LEGACY
							 PWM_GEN_MODE_DB_NO_SYNC
							 PWM_GEN_MODE_DB_SYNC_LOCAL
							 PWM_GEN_MODE_DB_SYNC_GLOBAL
							 PWM_GEN_MODE_GEN_NO_SYNC
							 PWM_GEN_MODE_GEN_SYNC_LOCAL
							 PWM_GEN_MODE_GEN_SYNC_GLOBAL */
	unsigned long period; /* ulPeriod = SysCtlClockGet() / 50000; */
	unsigned long width;
	unsigned long out; /* PWM_OUT_0
						  PWM_OUT_1
						  PWM_OUT_2
						  PWM_OUT_3
						  PWM_OUT_4
						  PWM_OUT_5
						  PWM_OUT_6
						  PWM_OUT_7
						*/
	unsigned long outbit; /* PWM_OUT_0_BIT
						  PWM_OUT_1_BIT
						  PWM_OUT_2_BIT
						  PWM_OUT_3_BIT
						  PWM_OUT_4_BIT
						  PWM_OUT_5_BIT
						  PWM_OUT_6_BIT
						  PWM_OUT_7_BIT */
	unsigned long trig; /* PWM_INT_CNT_ZERO
						   PWM_INT_CNT_LOAD
						   PWM_INT_CNT_AU
						   PWM_INT_CNT_AD
						   PWM_INT_CNT_BU
						   PWM_INT_CNT_BD
						   PWM_TR_CNT_ZERO
						   PWM_TR_CNT_LOAD
						   PWM_TR_CNT_AU
						   PWM_TR_CNT_AD
						   PWM_TR_CNT_BU
						   PWM_TR_CNT_BD */
	unsigned long intergen; /* PWM_INT_GEN_0
							   PWM_INT_GEN_1
							   PWM_INT_GEN_2
							   PWM_INT_GEN_3
							   PWM_INT_FAULT
							   PWM_INT_FAULT0
							   PWM_INT_FAULT1
							   PWM_INT_FAULT2
							   PWM_INT_FAULT3
							   PWM_INT_FAULT_M */
	unsigned long interrupt; /* INT_PWM0
								INT_PWM1
								INT_PWM2 */
	void (*handler)(void); /* PWMGenIntStatus(),
							  PWMGenIntClear() */
} PWM_t;

extern void PWM_init(PWM_t *pwm);

#endif /* __SYS_PWM_H__ */
