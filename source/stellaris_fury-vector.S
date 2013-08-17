/* Vector table for stellaris_fury
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
	.section	".cs3.interrupt_vector", "ax"
	.globl	__cs3_interrupt_vector_stellaris_fury
	.type	__cs3_interrupt_vector_stellaris_fury, %object
__cs3_interrupt_vector_stellaris_fury:
	.long	__cs3_stack
	.long	__cs3_reset
	.long	__cs3_isr_nmi
	.long	__cs3_isr_hard_fault
	.long	__cs3_isr_mpu_fault
	.long	__cs3_isr_bus_fault
	.long	__cs3_isr_usage_fault
	.long	__cs3_isr_reserved_7
	.long	__cs3_isr_reserved_8
	.long	__cs3_isr_reserved_9
	.long	__cs3_isr_reserved_10
	.long	__cs3_isr_svcall
	.long	__cs3_isr_debug
	.long	__cs3_isr_reserved_13
	.long	__cs3_isr_pendsv
	.long	__cs3_isr_systick
	.long	__cs3_isr_gpio_a
	.long	__cs3_isr_gpio_b
	.long	__cs3_isr_gpio_c
	.long	__cs3_isr_gpio_d
	.long	__cs3_isr_gpio_e
	.long	__cs3_isr_uart0
	.long	__cs3_isr_uart1
	.long	__cs3_isr_ssi0
	.long	__cs3_isr_i2c0
	.long	__cs3_isr_pwm_fault
	.long	__cs3_isr_pwm0
	.long	__cs3_isr_pwm1
	.long	__cs3_isr_pwm2
	.long	__cs3_isr_qei0
	.long	__cs3_isr_adc0
	.long	__cs3_isr_adc1
	.long	__cs3_isr_adc2
	.long	__cs3_isr_adc3
	.long	__cs3_isr_watchdog
	.long	__cs3_isr_timer0a
	.long	__cs3_isr_timer0b
	.long	__cs3_isr_timer1a
	.long	__cs3_isr_timer1b
	.long	__cs3_isr_timer2a
	.long	__cs3_isr_timer2b
	.long	__cs3_isr_comp0
	.long	__cs3_isr_comp1
	.long	__cs3_isr_comp2
	.long	__cs3_isr_sysctl
	.long	__cs3_isr_flashctl
	.long	__cs3_isr_gpio_f
	.long	__cs3_isr_gpio_g
	.long	__cs3_isr_gpio_h
	.long	__cs3_isr_uart2
	.long	__cs3_isr_ssi1
	.long	__cs3_isr_timer3a
	.long	__cs3_isr_timer3b
	.long	__cs3_isr_i2c1
	.long	__cs3_isr_qei1
	.long	__cs3_isr_can0
	.long	__cs3_isr_can1
	.long	__cs3_isr_can2
	.long	__cs3_isr_ethernet0
	.long	__cs3_isr_hibernate
	.size	__cs3_interrupt_vector_stellaris_fury, . - __cs3_interrupt_vector_stellaris_fury
