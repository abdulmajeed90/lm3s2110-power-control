#ifndef __IIC_H__
#define __IIC_H__

#define IIC_PERIPH	SYSCTL_PERIPH_GPIOC
#define IIC_PORT	GPIO_PORTC_BASE

#define IIC_SDA		GPIO_PIN_7
#define IIC_CLK		GPIO_PIN_6

#define IIC_CLK_B	GPIO_PIN_4
#define IIC_SDA_B	GPIO_PIN_5

#define SCL_B_H		GPIOPinWrite(IIC_PORT, IIC_CLK_B, 0xff)
#define SCL_B_L		GPIOPinWrite(IIC_PORT, IIC_CLK_B, 0x00)

#define SDA_B_H		GPIOPinWrite(IIC_PORT, IIC_SDA_B, 0xff)
#define SDA_B_L		GPIOPinWrite(IIC_PORT, IIC_SDA_B, 0x00)

#define SDA_B_READ	(GPIOPinRead(IIC_PORT, IIC_SDA_B) & IIC_SDA_B)

#define SDA_B_IN	SDA_B_In
#define SDA_B_OUT	SDA_B_Out


#define SCL_B_Out		GPIOPinTypeGPIOOutput(IIC_PORT, IIC_CLK_B)
#define SDA_B_In		GPIOPinTypeGPIOInput(IIC_PORT, IIC_SDA_B)
#define SDA_B_Out		GPIOPinTypeGPIOOutput(IIC_PORT, IIC_SDA_B)




#if 1
#define SCL_H		GPIOPinWrite(IIC_PORT, IIC_CLK, 0xff)
#define SCL_L		GPIOPinWrite(IIC_PORT, IIC_CLK, 0x00)

#define SDA_H		GPIOPinWrite(IIC_PORT, IIC_SDA, 0xff)
#define SDA_L		GPIOPinWrite(IIC_PORT, IIC_SDA, 0x00)

#define SDA_READ	(GPIOPinRead(IIC_PORT, IIC_SDA) & IIC_SDA)


#define SCL_Out		GPIOPinTypeGPIOOutput(IIC_PORT, IIC_CLK)		
#define SDA_In		GPIOPinTypeGPIOInput(IIC_PORT, IIC_SDA)
#define SDA_Out		GPIOPinTypeGPIOOutput(IIC_PORT, IIC_SDA)

#else

#define SCL_H
#define SCL_L

#define SDA_H
#define SDA_L

#define SDA_READ	1


#define SCL_Out
#define SDA_In
#define SDA_Out

#endif


#define SDA_IN	SDA_In
#define SDA_OUT	SDA_Out


// 定义I2C总线管脚名称
#define I2C_SCL     GPIO_PIN_6
#define I2C_SDA     GPIO_PIN_7


// 定义I2C工作状态
#define STAT_IDLE           0           /* 空闲状态         */
#define STAT_RECEIVE_START  2           /* 起始接收数据状态 */
#define STAT_RECEIVE_CONT   3           /* 继续接收数据状态 */
#define STAT_RECEIVE_LAST   4           /* 最后接收数据状态 */

// 定义LM75A相关名称
#define SLA_LM75A       (0x90>>1)
#define LM75A_REG_TMP   0x00


// 定义命令：先重复起始，再接收1字节数据，最后停止
#define I2CM_CMD_PSR    (I2C_MASTER_CS_STOP | I2C_MASTER_CS_START | I2C_MASTER_CS_RUN)

void start();
void stop();
void Send1byte(unsigned int  byte);
unsigned int Read1byte();
unsigned char ADSGetByte();


void startB();
void stopB();
void Send1byteB(unsigned int  byte);
unsigned char ADSGetByteB();


#endif /* __IIC_H__ */
