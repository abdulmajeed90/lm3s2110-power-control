#ifndef __ADS8361_H__
#define __ADS8361_H__

/* #define ADS_PERIPH	SYSCTL_PERIPH_GPIOA */
#define ADS_PORT	GPIO_PORTA_BASE

#define ADS_CS		GPIO_PIN_0
#define ADS_BUSY	GPIO_PIN_1
#define ADS_SA		GPIO_PIN_4
#define ADS_RD		GPIO_PIN_5
#define ADS_CLK		GPIO_PIN_2
#define ADS_DRDY	GPIO_PIN_3

#define ADS_IN_MASK	(ADS_SA | ADS_DRDY)
#define ADS_OUT_MASK	(ADS_CS | ADS_RD | ADS_CLK | ADS_BUSY)

#define ADS_BUSY_H	GPIOPinWrite(ADS_PORT, ADS_BUSY, 0xff)
#define ADS_BUSY_L	GPIOPinWrite(ADS_PORT, ADS_BUSY, 0x00)

#define ADS_CS_H	GPIOPinWrite(ADS_PORT, ADS_CS, 0xff)
#define ADS_CS_L	GPIOPinWrite(ADS_PORT, ADS_CS, 0x00)

#define ADS_RD_H	GPIOPinWrite(ADS_PORT, ADS_RD, 0xff)
#define ADS_RD_L	GPIOPinWrite(ADS_PORT, ADS_RD, 0x00)

#define ADS_CLK_H	GPIOPinWrite(ADS_PORT, ADS_CLK, 0xff)
#define ADS_CLK_L	GPIOPinWrite(ADS_PORT, ADS_CLK, 0x00)

#define ADS_BUSY_READ	(GPIOPinRead(ADS_PORT, ADS_BUSY) & ADS_BUSY)
#define ADS_SA_READ	(GPIOPinRead(ADS_PORT, ADS_SA) & ADS_SA)
#define ADS_DRDY_READ	(GPIOPinRead(ADS_PORT, ADS_DRDY) & ADS_DRDY)

void ads_init_gpio(void);
unsigned int ads_read(unsigned int port);

void ads_confige(unsigned int channel);
void ads_point_reg(void);
unsigned int ads_read(unsigned int channel);


#endif /* __ADS8361_H__ */
