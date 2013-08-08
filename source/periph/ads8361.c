/* driver for ads8361
 */
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "src/sysctl.h"
#include "src/systick.h"
#include "hw_ssi.h"
#include "src/debug.h"
#include "src/gpio.h"
#include "src/ssi.h"
#include "src/pin_map.h"
#include "periph/ads8361.h"
/* ADS_init_gpio() - initialize ADC gpio
 */
void ADS_init_gpio(void)
{
#if 1
	/* enable GPIO */
	SysCtlPeripheralEnable(ADS_PERIPH);
	/* configure GPIO in output */
	GPIOPinTypeGPIOOutput(ADS_PORT, ADS_OUT_MASK);
	/* configure serial pin in input */
	GPIOPinTypeGPIOInput(ADS_PORT, ADS_IN_MASK);
#else
	/* enable CS */
	SysCtlPeripheralEnable(ADS_PERIPH);
	GPIOPinTypeGPIOOutput(ADS_PORT, ADS_CS);
	/* enable BUSY */
	GPIOPinTypeGPIOInput(ADS_PORT, ADS_BUSY);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI);//　使能SSI功能模块
	SysCtlPeripheralEnable(SSI0CLK_PERIPH);//  使能GPIOA口

	/* 设置SPI为主机模式0,8位数据宽度，115200的波特率  */
	SSIConfig(SSI_BASE, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 9600, 8);
	/* SSIConfig(SSI_BASE, SSI_FRF_TI, SSI_MODE_MASTER, 9600, 21); */
	/* enable SSI */
	SSIEnable(SSI_BASE);
	/* 设定 GPIO A 2~5 引脚为使用外设功能 */
	GPIOPinTypeSSI(SSI0CLK_PORT, (SSI0CLK_PIN | SSI0FSS_PIN | 
				SSI0RX_PIN  | SSI0TX_PIN ) );
#endif
}


/* ADS_read(port,value) - read ADC value for ADS8361
 * @port:
 * @value
 * return 16 bit value.
 */
unsigned int ADS_read(unsigned int port)
{
	unsigned long value = 0;

#if 1	/* 模拟SPI与硬件SPI */
	unsigned int i;
	unsigned int rd = 0;
	unsigned int out_value = 0x10001000;

	/* ADS_RD_H; */
	ADS_CS_L;
	ADS_BUSY_H;

	while(ADS_DRDY_READ);

	for (i = 0; i < 16; i++) {
		if (out_value & 0x800) ADS_RD_H;
		else ADS_RD_L;

		ADS_CLK_H;

		out_value <<= 1;
		value <<= 1;
		if (ADS_SA_READ) value |= 0x01;

		ADS_CLK_L;

	}
	return value;

	/* ADS_CS_H; */
		

#else
	/* SPI分段 */
	unsigned long ch_tmp;

	ADS_CS_L;

	SSIDataPut(SSI_BASE, 0xff);
	SSIDataGet(SSI_BASE, &value);
	ch_tmp = value << 10;

	SSIDataPut(SSI_BASE, 0xff);
	SSIDataGet(SSI_BASE, &value);
	ch_tmp |= value << 2;

	SSIDataPut(SSI_BASE, 0xff);
	SSIDataGet(SSI_BASE, &value);
	ch_tmp |= value >> 6;

	value_array[0] = ch_tmp;
	
	/*
	if ((ch_tmp & 0xc0000) == 0x80000)
		value_array[1] = ch_tmp;
	else if ((ch_tmp & 0xc0000) == 0x0000)
		value_array[0] = ch_tmp;
	else
		value_array[2] = ch_tmp;
	*/


	ADS_CS_H;
#endif
}

