
#define uint unsigned int
#define uchar unsigned char
//**************************修改硬件时要修改的部分********************************
sbit EOC   = P1^0; 
sbit CLOCK = P1^1;     //输入、输出时钟端
sbit D_IN  = P1^2;     //数据输入端
sbit D_OUT = P1^3;     //数据输出端
sbit  _CS  = P1^4;
uchar i=0;
void t100() 
{ uchar i;
	for(i=0;i<100;i++)
	{}
}
uint read2543(uchar port) //CON_WORD为选择的通道，为0--10
{
	uint data ad;
	uchar data i;
	uchar data al=0,ah=0;
	CLOCK=0;
	_CS=0;
	port<<=4;
	for(i=0;i<4;i++)
	{
		D_IN=(bit)(port&0x80);
		CLOCK=1;
		CLOCK=0;
		port<<=1;
	}
	D_IN=0;
	for(i=0;i<8;i++)
	{
		CLOCK=1;
		CLOCK=0;
	}
	_CS=1;
	t100();
	_CS=0;
	for(i=0;i<4;i++)
	{
		D_OUT=1;
		CLOCK=1;
		ah<<=1;
		if(D_OUT)ah|=0x01;
		CLOCK=0;
	}
	for(i=0;i<8;i++)
	{
		D_OUT=1;
		CLOCK=1;
		al<<=1;
		if(D_OUT) al|=0x01;
		CLOCK=0;
	}
	_CS=1;
	ad=(uint)ah;
	ad<<=8;
	ad|=al;
	return(ad);
}
//读取平均值，形参：采集端口 ，采集次数
uint avr_values(uchar port,uchar times)
{ unsigned int sum=0,average=0;
	unsigned int get_values[]=0;
	for(i=0;i<times;i++)
	{
		get_values[i]=read2543(port);
		sum=sum+get_values[i];
	}
average=sum/times;
return average;
}
