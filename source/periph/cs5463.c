/*
 * cs5463.c - 2013年08月31日 10时16分14秒
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
#include "periph/cs5463.h"

//--------CS5460 CODE ----------------------------
/*CS5460内部寄存器的低六位*/
#define CONFIG            0x00  //配置寄存器
#define CYCLE_COUNT       0x0A  //周期计数 
#define LAST_CURRENT      0x0E  //上一次电流值
#define LAST_VOLT         0x10  //上一次电压值
#define LAST_POWER        0x12  //上一次的功率值
#define LAST_ENERGY       0x14  //上一周期的总能量值
#define LAST_CURRENT_RMS  0x16  //上一周期电流有效值
#define LAST_VOLT_RMS     0x18  //上一周期电压有效值
#define STATUS            0x1E  //状态寄存器
#define MASK              0x34  //中断屏蔽寄存器

#define REAL_POWER       0x14  //上一周期的总能量值
#define CURRENT_RMS  0x16  //上一周期电流有效值
#define VOLT_RMS     0x18  //上一周期电压有效值


#define POWER_ACTIVE 0x14  //

#define OPERATIONAL_MODE_REG 36//

#define CURRENT_GAIN 0x04  //
#define VOLTAGE_GAIN 0x08  //
#define CURRENT_AC_OFFSET 0x20  //
#define VOLTAGE_AC_OFFSET 0x22  //


#define VOLT_DC_OFFSET    0X06  //电压通道 直流 偏置寄存器
#define CURRENT_DC_OFFSET 0X02  //电流通道 直流 偏置寄存器
/*直流偏置(已校准)*/
#define CURRENT_DC      0xCC4ADD //电压偏置   
#define VOLT_DC         0X331396 //电流偏置
/*寄存器的读写*/
#define READ  0x00    //读
#define WRITE 0x40    //写

/*各寄存器复位后的初始值*/
#define CONFIG_INIT      0x000001   //配置寄存器
#define STATUS_INIT      0x000001   //状态寄存器
#define MASK_INIT        0x000000   //中断屏蔽寄存器
#define CYCLE_COUNT_INIT 0x000FA0   //周期计数寄存器

/*定义以上寄存器的初始化后的值*/
#define  CONFIG_DATA      0x0010E1  //软件复位
#define  CALCULATE_DOWN   0x800000  //数据已经就绪，转换结束
#define  LOW_POWER        0x000004  //检测到低电源
#define  WDT              0X000020  //能量超过5秒未被读取，该位置位 

/*定义其他命令*/
#define  POWER_OFF_CTRL  0X88  //掉电控制
#define  STANDERED_CTRL  0XC0  //标准控制
#define  TRANSFORM_START 0Xe8 //开始转换,执行连续计算周期


//power up/halt
#define  POWER_HALT_UP	0xA0



//calibraton type
#define  CALIBRA_DC_CURRENT_OFFSET 0XC1


#define  CALIBRA_AC_CURRENT_OFFSET 0XCD
#define  CALIBRA_AC_CURRENT_GAIN 0XCE
#define  CALIBRA_AC_VOLTAGE_OFFSET 0Xd5
#define  CALIBRA_AC_VOLTAGE_GAIN 0Xd6
#define  CALIBRA_AC_VOL_CURRENT_OFFSET 0xDD
#define  CALIBRA_AC_VOL_CURRENT_GAIN 	0xDE
//#define  CALIBRA_AC_VOLTAGE_OFFSET 0Xd5
//#define  CALIBRA_AC_VOLTAGE_GAIN 0Xd6

//#define  CALIBRA_AC_CURRENT_OFFSET 0XCD


//#define  CS5463_READ_TIMER 50//500ms to read data one time!
#define  CS5463_READ_TIMER 100//500ms to read data one time!
//#define  CS5463_READ_TIMER 200

#define  CURRENT_RANGE	30
#define  VOLTAGE_RANGE	250
#define  POWER_RANGE	(CURRENT_RANGE*VOLTAGE_RANGE)

#define  ACT_PWR_SMP_SUPPORTED_QTY 3




#define SPI_PERIPH		SYSCTL_PERIPH_GPIOB
#define SPI_BASE		GPIO_PORTB_BASE
#define SPI_SCK			GPIO_PIN_1
#define SPI_MISO		GPIO_PIN_2
#define SPI_MOSI		GPIO_PIN_3

#define SPI_SCK_H		GPIOPinWrite(SPI_BASE, SPI_SCK, 0xff)
#define SPI_SCK_L		GPIOPinWrite(SPI_BASE, SPI_SCK, 0x00)
                    	
#define SPI_MOSI_H		GPIOPinWrite(SPI_BASE, SPI_MOSI, 0xff)
#define SPI_MOSI_L		GPIOPinWrite(SPI_BASE, SPI_MOSI, 0x00)

#define SPI_MISO_READ	(GPIOPinRead(SPI_BASE, SPI_MISO) & SPI_MISO)


/* cs5463_init -
 */
void cs5463_init_gpio(void)
{
	/* Configure the GPIO */
	SysCtlPeripheralEnable(SPI_PERIPH);
	GPIOPinTypeGPIOOutput(SPI_BASE, SPI_SCK | SPI_MOSI);
	GPIOPinTypeGPIOInput(SPI_BASE, SPI_MISO);
}		/* -----  end of function cs5463_init  ----- */

/* cs5463_write -
*/
void cs5463_write(unsigned char *dat, int num)
{
	unsigned char i;

	while (num--) {
		for (i=0; i<8; i++) {
			SPI_SCK_L;
			if (*dat & 0x80) {
				SPI_MOSI_H;
			} else {
				SPI_MOSI_L;
			}
			SPI_SCK_H;
			*dat <<= 1;
		}
		dat++;
	}
	SPI_SCK_L;
}		/* -----  end of function cs5463_write  ----- */

/* cs5463_read -
*/
void cs5463_read(unsigned char *dat, unsigned char reg)
{
	unsigned char i, j;

	cs5463_write(&reg, 1);
	SPI_MOSI_H;

	for (j=0; j<3; j++) {
		for (i=0; i<8; i++) {
			*dat <<= 1;
			SPI_SCK_L;
			SPI_SCK_H;
			if (SPI_MISO_READ) {
				*dat |= 0x01;
			}
		}
		SPI_SCK_L;
		SPI_SCK_H;
		dat++;
	}
}		/* -----  end of function cs5463_read  ----- */

/* cs5463_init -
*/
void cs5463_init(void)
{
	unsigned char buff[4];

	cs5463_init_gpio();
	
	/* Configure command */
	buff[0] = 0xff;
	buff[1] = 0xff;
	buff[2] = 0xff;
	buff[3] = 0xfe;
	cs5463_write(buff, 4);

	buff[0] = 0x40;
	buff[1] = 0x01;
	buff[2] = 0x10;
	buff[3] = 0x68;
	cs5463_write(buff, 4);

	buff[0] = WRITE|CYCLE_COUNT;
	buff[1] = 0x00;
	buff[2] = 0x01;
	buff[3] = 0xf4;
	cs5463_write(buff, 4);

}		/* -----  end of function cs5463_init  ----- */
