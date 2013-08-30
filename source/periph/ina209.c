/*
 * ina209.c - 2013年08月29日 15时02分19秒
 *
 * Copyright (c) 2013, chenchacha 
 */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "src/sysctl.h"
#include "src/gpio.h"
#include "src/i2c.h"
#include "src/interrupt.h"
#include "src/pin_map.h"

#include "delay.h"
#include "periph/iic.h"
#include "periph/ina209.h"

/* ina_init - initialize ina209
 */
void ina_init(void)
{
	SysCtlPeripheralEnable(IIC_PERIPH);
	SDA_Out;
	SCL_Out;
	SCL_H;
	SDA_H;
}		/* -----  end of function ina_init  ----- */

/* ina_write - write data to ina209
 */
void ina_write(unsigned char addr, unsigned char reg, unsigned int dat)
{
	start();
	iic_write_m(addr);
	iic_write_m(reg);
	iic_write_m(dat >> 8);
	iic_write_m(dat & 0xff);
	stop();
}		/* -----  end of function ina_write  ----- */

/* ina_read - read data from ina209
 */
unsigned int ina_read(unsigned char addr, unsigned char reg)
{
	unsigned int dat;

	start();
	iic_write_m(addr);
	iic_write_m(reg);
	stop();

	start();
	iic_write_m(addr+1);
	dat = iic_read_m();
	dat <<= 8;
	dat |= iic_read_m();
	stop();

	return dat;
}		/* -----  end of function ina_read  ----- */

/* ina_set_reg -
 */
void ina_set_reg(unsigned char addr,
		unsigned int configure,
		unsigned int calibration)
{
	ina_write(addr, CONFIGURE_REG, configure);
	ina_write(addr, CALIBRATION, calibration);
}		/* -----  end of function ina_set_reg  ----- */


/* ina_status - check the status
 */
inline int ina_status(unsigned short status)
{
	return (ina_read(INA_ADDRESS, STATUS_REG) & status);
}		/* -----  end of function ina_status  ----- */

/* ina_scan_status -
 */
unsigned short ina_scan_status(void)
{
	unsigned i;

	/* Scan the status register */
	for (i=3; i<16; i++) {
		if (ina_status(0x01 << i))
			return i;
	}
	/* No warning */
	return 0;
}		/* -----  end of function ina_scan_status  ----- */
