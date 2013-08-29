#ifndef __IIC_H__
#define __IIC_H__

#define IIC_PERIPH	SYSCTL_PERIPH_GPIOD
#define IIC_PORT	GPIO_PORTD_BASE

#define IIC_CLK		GPIO_PIN_7
#define IIC_SDA		GPIO_PIN_6

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




#define SCL_H		GPIOPinWrite(IIC_PORT, IIC_CLK, 0xff)
#define SCL_L		GPIOPinWrite(IIC_PORT, IIC_CLK, 0x00)

#define SDA_H		GPIOPinWrite(IIC_PORT, IIC_SDA, 0xff)
#define SDA_L		GPIOPinWrite(IIC_PORT, IIC_SDA, 0x00)

#define SDA_READ	(GPIOPinRead(IIC_PORT, IIC_SDA) & IIC_SDA)


#define SCL_Out		GPIOPinTypeGPIOOutput(IIC_PORT, IIC_CLK)		
#define SDA_In		GPIOPinTypeGPIOInput(IIC_PORT, IIC_SDA)
#define SDA_Out		GPIOPinTypeGPIOOutput(IIC_PORT, IIC_SDA)


#define SDA_IN	SDA_In
#define SDA_OUT	SDA_Out





// 定义LM75A相关名称
#define SLA_LM75A       (0x90>>1)
#define LM75A_REG_TMP   0x00


// 定义命令：先重复起始，再接收1字节数据，最后停止
#define I2CM_CMD_PSR    (I2C_MASTER_CS_STOP | I2C_MASTER_CS_START | I2C_MASTER_CS_RUN)

extern void start();
extern void stop();
extern void iic_write_m(unsigned char byte); 
extern void Send1byte(unsigned int  byte);
extern unsigned int Read1byte();
extern unsigned char ADSGetByte();
extern unsigned char iic_read_m();
extern void startB();
extern void stopB();
extern void Send1byteB(unsigned int  byte);
extern unsigned char ADSGetByteB();
extern unsigned int ads_read_count(unsigned int count);

/* The states in the interrupt handler state machine */
#define STATE_IDLE         0
#define STATE_WRITE_NEXT   1
#define STATE_WRITE_FINAL  2
#define STATE_WAIT_ACK     3
#define STATE_SEND_ACK     4
#define STATE_READ_ONE     5
#define STATE_READ_FIRST   6
#define STATE_READ_NEXT    7
#define STATE_READ_FINAL   8
#define STATE_READ_WAIT    9

/* IIC status struct */
typedef struct {
	unsigned long status;
	unsigned long count;
	unsigned char *pdat;
} IIC_t;

extern void iic_sys_init(void);
extern void iic_read(unsigned long address, unsigned char *dat, unsigned long count);
extern void iic_write(unsigned long address, unsigned char *dat, unsigned long count);

#endif /* __IIC_H__ */
