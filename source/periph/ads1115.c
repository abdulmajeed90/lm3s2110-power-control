/* driver for ads1115
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

#include "periph/iic.h"

/* extern unsigned int Initdata[4]; */
unsigned int result_gd=0;


void ads_confige(unsigned int channel)
{
#if 1
	SysCtlPeripheralEnable(IIC_PERIPH);
	SDA_Out;
	SCL_Out;
	SCL_H;
	SDA_H;

	unsigned char i=0;
	unsigned char Initdata[4]={0};

	switch (channel) {
		case 0:
			Initdata[2] = 0xc2;
			break;
		case 1:
			Initdata[2] = 0xd2;
			break;
		case 2:
			Initdata[2] = 0xe2;
			break;
		case 3:
			Initdata[2] = 0xf2;
			break;
		default:
			break;
	}

	/* address and write command */
	Initdata[0] =0x90;
	/* point register */
	Initdata[1] =0x01;
	Initdata[3] =0xe3;
	/* Initdata[3] =0x03; // 配置字低字节 */

	start();
	for(i=0;i<4;i++)
	{
		Send1byte(Initdata[i]);
	}

	stop();
#endif
}

void ads_point_reg(void)
{
	unsigned char i=0;
	unsigned char Initdata[4]={0};

	/* address and write command */
	Initdata[0] =0x90;
	/* point register */
	Initdata[1] =0x00;

	SDA_OUT;
	start();
	for(i=0;i<2;i++)
	{
		Send1byte(Initdata[i]);
	}

	/* IIC_write(Initdata, 0, 16); */

	stop();
}

unsigned int ads_read(unsigned int channel)
{
	unsigned short int result=0;

	if (channel > 4)
		channel = 0;

	/* Confige1115A(0); */
	ads_confige(channel);
	/* Pointregister(); */
	ads_point_reg();

	/* delay_ads(100); */

	start();
	Send1byte(0x91); //地址+读
	result|=ADSGetByte()<<8;
	result|=ADSGetByte();
	stop();
	return result;
}


/*
 * ADS1115补救程序
 */


void Confige_1115_B(unsigned int channel)
{
	SysCtlPeripheralEnable(IIC_PERIPH);
	SDA_B_Out;
	SCL_B_Out;
	SCL_B_H;
	SDA_B_H;

	unsigned char i=0;
	unsigned char Initdata[4]={0};

	switch (channel) {
		case 0:
			Initdata[2] = 0xc2;
			break;
		case 1:
			Initdata[2] = 0xd2;
			break;
		case 2:
			Initdata[2] = 0xe2;
			break;
		case 3:
			Initdata[2] = 0xf2;
			break;
		default:
			break;
	}

	Initdata[0] =0x90; // 地址 ＋ 写命令
	Initdata[1] =0x01; // 指向配置寄存器
	Initdata[3] =0xe3;
	/* Initdata[3] =0x03; // 配置字低字节 */

	startB();
	for(i=0;i<4;i++)
	{
		Send1byteB(Initdata[i]);
	}

	stopB();
}
void Point_reg_B(void)
{
	unsigned char i=0;
	unsigned char Initdata[4]={0};

	Initdata[0] =0x90; // 地址 ＋ 写命令
	Initdata[1] =0x00; // 指向转换结果寄存器

	SDA_B_OUT;
	startB();
	for(i=0;i<2;i++)
	{
		Send1byteB(Initdata[i]);
	}

	/* IIC_write(Initdata, 0, 16); */

	stopB();
}

unsigned int Read_1115_B(unsigned int channel)
{
	unsigned short int Result=0;

	/* Confige1115A(0); */
	Confige_1115_B(channel);
	/* Pointregister(); */
	Point_reg_B();

	/* delay_ads(100); */

	startB();
	Send1byteB(0x91); //地址+读
	Result|=ADSGetByteB()<<8;
	Result|=ADSGetByteB();
	stopB();
	return Result;
}

