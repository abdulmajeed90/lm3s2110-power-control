/*
 * ina209.h - 2013年08月29日 16时00分36秒
 *
 * Copyright (c) 2013, chenchacha 
 */
#ifndef  __INA209_H__
#define  __INA209_H__

/* The chip register address */
#define CONFIGURE_REG				0x00
#define STATUS_REG					0x01	/* RO */
#define SMBUS_CONTROL				0x02
#define SHUNT_VOLTAGE				0x03	/* RO */
#define BUS_VOLTAGE					0x04	/* RO */
#define POWER_REG					0x05	/* RO */
#define CURRENT_REG					0x06 	/* RO */
#define SHUNT_POSITIVE_PEAK			0x07
#define SHUNT_NEGATIVE_PEAK			0x08
#define BUS_MAX_PEAK				0x09
#define BUS_MIN_PEAK				0x0a
#define POWER_PEAK					0x0b
#define SHUNT_POSITIVE_WARNING		0x0c
#define SHUNT_NEGATIVE_WARNING		0x0d
#define POWER_WARNING				0x0e
#define BUS_OVER_WARNING			0x0f
#define BUS_UNDER_WARNING			0x10
#define POWER_OVER_LIMIT			0x11
#define BUS_OVER_VOLTAGE			0x12
#define BUS_UNDER_VOLTAGE			0x13
#define CRITICAL_DAC_POSTIVE		0x14
#define CRITICAL_DAC_NEGATIVE		0x15
#define CALIBRATION					0x16

#define RST							0x8000  
                                	
#define BUS_VOL_RAN_16 				0x0000
#define BUS_VOL_RAN_32 				0x2000
                                	
#define PGA_GAIN_40 				0x0000
#define PGA_GAIN_80 				0x0800
#define PGA_GAIN_160 				0x1000
#define PGA_GAIN_320 				0x1800
                                	
#define BADC_9_BIT	 				0x0000
#define BADC_10_BIT	 				0x0080
#define BADC_11_BIT	 				0x0100
#define BADC_12_BIT	 				0x0180
                                	
#define BADC_2_SAMPLE				0x0480
#define BADC_4_SAMPLE				0x0500
#define BADC_8_SAMPLE				0x0580
#define BADC_16_SAMPLE				0x0600
#define BADC_32_SAMPLE				0x0680
#define BADC_64_SAMPLE				0x0700
#define BADC_128_SAMPLE				0x0780
                                	
#define SADC_9_BIT	 				0x0000
#define SADC_10_BIT	 				0x0008
#define SADC_11_BIT	 				0x0010
#define SADC_12_BIT	 				0x0018
                                	
#define SADC_2_SAMPLE				0x0048
#define SADC_4_SAMPLE				0x0050
#define SADC_8_SAMPLE				0x0058
#define SADC_16_SAMPLE				0x0060
#define SADC_32_SAMPLE				0x0068
#define SADC_64_SAMPLE				0x0070
#define SADC_128_SAMPLE				0x0078
                                	
#define MODE_POWER_DOWN				0x0000 
#define MODE_SHUNT_TRIG				0x0001 
#define MODE_BUS_TRIG				0x0002
#define MODE_SHUNT_BUS_TRIG			0x0003
#define MODE_ADC_OFF				0x0004
#define MODE_SHUNT_CON				0x0005
#define MODE_BUS_CON				0x0006
#define MODE_SHUNT_BUS_CON			0x0007

#define W_BUS_OVERV					(0x01 << 15)
#define W_BUS_UNDERV				(0x01 << 14)
#define W_POWER						(0x01 << 13)
#define W_SHUNT_PV					(0x01 << 12)
#define W_SHUNT_NV					(0x01 << 11)
#define W_OL_BUS_OVERV				(0x01 << 10)
#define W_OL_BUS_UNDER				(0x01 << 9)
#define W_OL_POWER					(0x01 << 8)
#define W_CRIT_SHUNT_PV				(0x01 << 7)
#define W_CRIT_SHUNT_NV				(0x01 << 6)
#define W_CONVER_READY				(0x01 << 5)
#define W_SMBA						(0x01 << 4)
#define W_MATH_OVER					(0x01 << 3)


//INA209 Configuration 

#define INA_ADDRESS			0x80 
/* #define INA_CAL			 	0x3556 
 */
/* #define INA_CAL				3548
 */
/* #define INA_CAL				3462
 */
/* #define INA_CAL				4096
 * #define CUR_LSB				0x2
 */
#define INA_CAL					0x2800
#define CURRENT_LSB				2
#define POWER_LSB				4
#define INA_CONF			(BUS_VOL_RAN_32|PGA_GAIN_80|BADC_128_SAMPLE|SADC_128_SAMPLE|MODE_SHUNT_BUS_CON) 

/* #define INA_CONF			(BUS_VOL_RAN_32| \
 * 		                     PGA_GAIN_320| \
 * 		                     BADC_12_BIT| \
 * 		                     SADC_12_BIT| \
 * 		                     MODE_SHUNT_BUS_CON) 
 */

/* 
 * #define Own_Adress			0x01
 * #define Cur_LSB				0x0003		// in 10uA
 * #define Pow_LSB				(2*Cur_LSB) // in 100uW
 * #define BusVolOffset		0x34		// in mV 
 * 
 */



extern void ina_init(void);
extern void ina_write(unsigned char addr, unsigned char reg, unsigned int dat);
extern unsigned int ina_read(unsigned char addr, unsigned char reg);
extern void ina_set_reg(unsigned char addr,
		unsigned int configure,
		unsigned int calibration);
extern int ina_status(unsigned short status);
extern unsigned short ina_scan_status(void);
#endif   /* ----- #ifndef __INA209_H__  ----- */
