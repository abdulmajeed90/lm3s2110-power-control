/* ISRs for stellaris_fury
 *
 * Version: Sourcery CodeBench 2013.05-28
 * Support: https://sourcery.mentor.com/GNUToolchain/
 *
 * Copyright (c) 2007-2010 CodeSourcery, Inc.
 * Copyright (c) 2010-2013 Mentor Graphics, Inc.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 */
	.thumb

#if defined (L_stellaris_fury_isr_interrupt)
	.globl	__cs3_isr_interrupt
	.type	__cs3_isr_interrupt, %function
__cs3_isr_interrupt:
	b .
	.size	__cs3_isr_interrupt, . - __cs3_isr_interrupt

	.weak	__cs3_isr_reserved_7
	.globl	__cs3_isr_reserved_7
	.set	__cs3_isr_reserved_7, __cs3_isr_interrupt
	.weak	__cs3_isr_reserved_8
	.globl	__cs3_isr_reserved_8
	.set	__cs3_isr_reserved_8, __cs3_isr_interrupt
	.weak	__cs3_isr_reserved_9
	.globl	__cs3_isr_reserved_9
	.set	__cs3_isr_reserved_9, __cs3_isr_interrupt
	.weak	__cs3_isr_reserved_10
	.globl	__cs3_isr_reserved_10
	.set	__cs3_isr_reserved_10, __cs3_isr_interrupt
	.weak	__cs3_isr_reserved_13
	.globl	__cs3_isr_reserved_13
	.set	__cs3_isr_reserved_13, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_a
	.globl	__cs3_isr_gpio_a
	.set	__cs3_isr_gpio_a, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_b
	.globl	__cs3_isr_gpio_b
	.set	__cs3_isr_gpio_b, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_c
	.globl	__cs3_isr_gpio_c
	.set	__cs3_isr_gpio_c, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_d
	.globl	__cs3_isr_gpio_d
	.set	__cs3_isr_gpio_d, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_e
	.globl	__cs3_isr_gpio_e
	.set	__cs3_isr_gpio_e, __cs3_isr_interrupt
	.weak	__cs3_isr_uart0
	.globl	__cs3_isr_uart0
	.set	__cs3_isr_uart0, __cs3_isr_interrupt
	.weak	__cs3_isr_uart1
	.globl	__cs3_isr_uart1
	.set	__cs3_isr_uart1, __cs3_isr_interrupt
	.weak	__cs3_isr_ssi0
	.globl	__cs3_isr_ssi0
	.set	__cs3_isr_ssi0, __cs3_isr_interrupt
	.weak	__cs3_isr_i2c0
	.globl	__cs3_isr_i2c0
	.set	__cs3_isr_i2c0, __cs3_isr_interrupt
	.weak	__cs3_isr_pwm_fault
	.globl	__cs3_isr_pwm_fault
	.set	__cs3_isr_pwm_fault, __cs3_isr_interrupt
	.weak	__cs3_isr_pwm0
	.globl	__cs3_isr_pwm0
	.set	__cs3_isr_pwm0, __cs3_isr_interrupt
	.weak	__cs3_isr_pwm1
	.globl	__cs3_isr_pwm1
	.set	__cs3_isr_pwm1, __cs3_isr_interrupt
	.weak	__cs3_isr_pwm2
	.globl	__cs3_isr_pwm2
	.set	__cs3_isr_pwm2, __cs3_isr_interrupt
	.weak	__cs3_isr_qei0
	.globl	__cs3_isr_qei0
	.set	__cs3_isr_qei0, __cs3_isr_interrupt
	.weak	__cs3_isr_adc0
	.globl	__cs3_isr_adc0
	.set	__cs3_isr_adc0, __cs3_isr_interrupt
	.weak	__cs3_isr_adc1
	.globl	__cs3_isr_adc1
	.set	__cs3_isr_adc1, __cs3_isr_interrupt
	.weak	__cs3_isr_adc2
	.globl	__cs3_isr_adc2
	.set	__cs3_isr_adc2, __cs3_isr_interrupt
	.weak	__cs3_isr_adc3
	.globl	__cs3_isr_adc3
	.set	__cs3_isr_adc3, __cs3_isr_interrupt
	.weak	__cs3_isr_watchdog
	.globl	__cs3_isr_watchdog
	.set	__cs3_isr_watchdog, __cs3_isr_interrupt
	.weak	__cs3_isr_timer0a
	.globl	__cs3_isr_timer0a
	.set	__cs3_isr_timer0a, __cs3_isr_interrupt
	.weak	__cs3_isr_timer0b
	.globl	__cs3_isr_timer0b
	.set	__cs3_isr_timer0b, __cs3_isr_interrupt
	.weak	__cs3_isr_timer1a
	.globl	__cs3_isr_timer1a
	.set	__cs3_isr_timer1a, __cs3_isr_interrupt
	.weak	__cs3_isr_timer1b
	.globl	__cs3_isr_timer1b
	.set	__cs3_isr_timer1b, __cs3_isr_interrupt
	.weak	__cs3_isr_timer2a
	.globl	__cs3_isr_timer2a
	.set	__cs3_isr_timer2a, __cs3_isr_interrupt
	.weak	__cs3_isr_timer2b
	.globl	__cs3_isr_timer2b
	.set	__cs3_isr_timer2b, __cs3_isr_interrupt
	.weak	__cs3_isr_comp0
	.globl	__cs3_isr_comp0
	.set	__cs3_isr_comp0, __cs3_isr_interrupt
	.weak	__cs3_isr_comp1
	.globl	__cs3_isr_comp1
	.set	__cs3_isr_comp1, __cs3_isr_interrupt
	.weak	__cs3_isr_comp2
	.globl	__cs3_isr_comp2
	.set	__cs3_isr_comp2, __cs3_isr_interrupt
	.weak	__cs3_isr_sysctl
	.globl	__cs3_isr_sysctl
	.set	__cs3_isr_sysctl, __cs3_isr_interrupt
	.weak	__cs3_isr_flashctl
	.globl	__cs3_isr_flashctl
	.set	__cs3_isr_flashctl, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_f
	.globl	__cs3_isr_gpio_f
	.set	__cs3_isr_gpio_f, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_g
	.globl	__cs3_isr_gpio_g
	.set	__cs3_isr_gpio_g, __cs3_isr_interrupt
	.weak	__cs3_isr_gpio_h
	.globl	__cs3_isr_gpio_h
	.set	__cs3_isr_gpio_h, __cs3_isr_interrupt
	.weak	__cs3_isr_uart2
	.globl	__cs3_isr_uart2
	.set	__cs3_isr_uart2, __cs3_isr_interrupt
	.weak	__cs3_isr_ssi1
	.globl	__cs3_isr_ssi1
	.set	__cs3_isr_ssi1, __cs3_isr_interrupt
	.weak	__cs3_isr_timer3a
	.globl	__cs3_isr_timer3a
	.set	__cs3_isr_timer3a, __cs3_isr_interrupt
	.weak	__cs3_isr_timer3b
	.globl	__cs3_isr_timer3b
	.set	__cs3_isr_timer3b, __cs3_isr_interrupt
	.weak	__cs3_isr_i2c1
	.globl	__cs3_isr_i2c1
	.set	__cs3_isr_i2c1, __cs3_isr_interrupt
	.weak	__cs3_isr_qei1
	.globl	__cs3_isr_qei1
	.set	__cs3_isr_qei1, __cs3_isr_interrupt
	.weak	__cs3_isr_can0
	.globl	__cs3_isr_can0
	.set	__cs3_isr_can0, __cs3_isr_interrupt
	.weak	__cs3_isr_can1
	.globl	__cs3_isr_can1
	.set	__cs3_isr_can1, __cs3_isr_interrupt
	.weak	__cs3_isr_can2
	.globl	__cs3_isr_can2
	.set	__cs3_isr_can2, __cs3_isr_interrupt
	.weak	__cs3_isr_ethernet0
	.globl	__cs3_isr_ethernet0
	.set	__cs3_isr_ethernet0, __cs3_isr_interrupt
	.weak	__cs3_isr_hibernate
	.globl	__cs3_isr_hibernate
	.set	__cs3_isr_hibernate, __cs3_isr_interrupt
#endif /* interrupt */

#if defined (L_stellaris_fury_isr_nmi)
	.globl	__cs3_isr_nmi
	.type	__cs3_isr_nmi, %function
__cs3_isr_nmi:
	b .
	.size	__cs3_isr_nmi, . - __cs3_isr_nmi

#endif /* nmi */

#if defined (L_stellaris_fury_isr_hard_fault)
	.globl	__cs3_isr_hard_fault
	.type	__cs3_isr_hard_fault, %function
__cs3_isr_hard_fault:
	b .
	.size	__cs3_isr_hard_fault, . - __cs3_isr_hard_fault

#endif /* hard_fault */

#if defined (L_stellaris_fury_isr_mpu_fault)
	.globl	__cs3_isr_mpu_fault
	.type	__cs3_isr_mpu_fault, %function
__cs3_isr_mpu_fault:
	b .
	.size	__cs3_isr_mpu_fault, . - __cs3_isr_mpu_fault

#endif /* mpu_fault */

#if defined (L_stellaris_fury_isr_bus_fault)
	.globl	__cs3_isr_bus_fault
	.type	__cs3_isr_bus_fault, %function
__cs3_isr_bus_fault:
	b .
	.size	__cs3_isr_bus_fault, . - __cs3_isr_bus_fault

#endif /* bus_fault */

#if defined (L_stellaris_fury_isr_usage_fault)
	.globl	__cs3_isr_usage_fault
	.type	__cs3_isr_usage_fault, %function
__cs3_isr_usage_fault:
	b .
	.size	__cs3_isr_usage_fault, . - __cs3_isr_usage_fault

#endif /* usage_fault */

#if defined (L_stellaris_fury_isr_svcall)
	.globl	__cs3_isr_svcall
	.type	__cs3_isr_svcall, %function
__cs3_isr_svcall:
	b .
	.size	__cs3_isr_svcall, . - __cs3_isr_svcall

#endif /* svcall */

#if defined (L_stellaris_fury_isr_debug)
	.globl	__cs3_isr_debug
	.type	__cs3_isr_debug, %function
__cs3_isr_debug:
	b .
	.size	__cs3_isr_debug, . - __cs3_isr_debug

#endif /* debug */

#if defined (L_stellaris_fury_isr_pendsv)
	.globl	__cs3_isr_pendsv
	.type	__cs3_isr_pendsv, %function
__cs3_isr_pendsv:
	b .
	.size	__cs3_isr_pendsv, . - __cs3_isr_pendsv

#endif /* pendsv */

#if defined (L_stellaris_fury_isr_systick)
	.globl	__cs3_isr_systick
	.type	__cs3_isr_systick, %function
__cs3_isr_systick:
	b .
	.size	__cs3_isr_systick, . - __cs3_isr_systick

#endif /* systick */
