#include <msp430g2553.h>

#define SCL_OUT P2DIR |= BIT0;

#define SDA_OUT P2DIR |= BIT1;
#define SDA_IN P2DIR &= ~BIT1;

#define SCL_0 P2OUT &= ~BIT0;
#define SCL_1 P2OUT |= BIT0;
#define SDA_0 P2OUT &= ~BIT1;
#define SDA_1 P2OUT |= BIT1;

#define channel_0 0
#define channel_1 1
#define channel_2 2
#define channel_3 3

unsigned char initdata[4] = {0};
unsigned char initdata[15] = {0};

void delay_us(unsigned char d)
{
	while(--d);
}

/**************开始ADC通道****************/
/*****************************************/
void startAD1com()
{
	SDA_0;
	SCL_1;
	delay_us(100);
	SDA_1;
	delay_us(20);
	SDA_0;
	delay_us(20);
	SCL_0;
	delay_us(10);
}
/**************停止ADC通道****************/
/*****************************************/
void stopAD1com()
{
	SDA_0;
	delau_us(10);
	SCL_1;
	delay_us(20);
	SDA_1;
	delay_us(10);
	//SCL_0;
	//delay_us(20);
}
/**************单字节SPI通道****************/
/*******************************************/
void send_byte(unsigned char byte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		SDA_0;
		if((byte << i) & 0x80) SDA_1;
		delay_us(5);
		SCL_1;
		delay_us(10);
		SCL_0;
		delay_us(10);
	}
	delay_us(5);
	SDA_1;
	delay_us(5);
	SCL_1;
	delay_us(10);
	SCL_0;
	delay_us(10);
}
/**************???????????****************/
/*****************************************/
void confige1115A(unsigned char channel_co)
{
	unsigned char i;
	unsigned char channel;
	switch(channel_co)
	{
		case 0: channel = 0xc0; //0通道
		case 1: channel = 0xD0; //1通道
		case 2: channel = 0xE0; //2通道
		case 3: channel = 0xF0; //3通道
	}
	initdata[0] = 0x90; //地址+写命令
	initdata[1] = 0x01; //指向配置寄存器
	//initdata[2] = 0x82; //配置字高字节
	initdata[2] = channel + 0x06;//配置高字节
	initdata[3] = 0xE3; //配置低字节
	P2DIR |= 0x02; //SDA脚设置推挽
	SCL_1;
	startAD1com();
	for(i=0;i<4;i++)
	{
		send_byte(initdata[i]);
		delay_us(20);
	}
	stopAD1com();
	P2DIR &= ~0x02; //SDA1 脚设置漏开
}
/**************读取一个字节***************/
/*****************************************/
unsigned char byte()
{
	unsigned char temp=0;
	unsigned char i;
	P2DIR &= ~0x02; //SDA1 脚设置漏开
	for(i=0;i<8;i++)
	{
		temp = temp << 1;
		SCL_0;
		delay_us(10);
		SCL_1;
		_NOP();
		delay_us(5);
		if(P2IN & 0x02) temp |= 0x01;
		_NOP();
		delay_us(10);
	}
	SCL_0;
	delay_us(5);
	P2DIR |= 0x02; //SDA1脚设置推挽
	SDA_0;
	delay_us(5);
	SCL_1;
	delay_us(20);
	SCL_0;
	delay_us(5);
	SDA_1;
	P2OUT &= ~0x02; //SDA1 脚设置漏开
	return temp;
}
/***********指向转换结果寄存器************/
/*****************************************/
void point_ster()
{
	unsigned char i=0;

	P2DIR |= 0x02; //SDA1 脚设置推挽
	SCL_0;
	startAD1com();
	for(i=0;i<2;i++)
	{
		send_byte(initdata[i]);
		delay_us(20);
	}
	stopAD1com();
	delay_us(10);
}
/**************读取AD转换值***************/
/*****************************************/
unsigned int read1115()
{
	unsigned char tL,tH;
	unsigned int result;

	initdata[0] = 0x91; //地址+读命令
	P2DIR |= 0x02; //SDA1脚设置推挽
	SCl_1;
	startAD1com();
	delay_us(10);
	send_byte(initdata[0]);
	P2DIR &= ~0x02; //SDA1脚设置漏开
	delay_us(20);
	tH = read_byte();
	delay_us(10);
	tL = read_byte();
	stopAD1com();
	result = tH + tL;
	return result;
}
/*************模拟量转化数字量************/
/*****************************************/
unsigned int AD_1115(unsigned char channel_Ad)
{
	unsigned int result_gd;
	confige1115A(channel_Ad);
	delay_us(1000);
	point_ster();
	delay_us(10);
	result_gd = read1115();
	return result_gd;
}


