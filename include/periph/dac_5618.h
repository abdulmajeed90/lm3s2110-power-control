#ifndef __DAC_5618_H__
#define __DAC_5618_H__

#define DAC_PERIPH	SYSCTL_PERIPH_GPIOC
#define DAC_PORT	GPIO_PORTC_BASE

#define DAC_DIN		GPIO_PIN_7
#define DAC_CLK		GPIO_PIN_6
#define DAC_CS		GPIO_PIN_5
#define DAC_PORT_MASK	(DAC_DIN | DAC_CLK | DAC_CS)

#define DAC_DAT_H	GPIOPinWrite(DAC_PORT, DAC_DIN, 0xff)
#define DAC_DAT_L	GPIOPinWrite(DAC_PORT, DAC_DIN, 0x00)

#define DAC_CLK_H	GPIOPinWrite(DAC_PORT, DAC_CLK, 0xff)
#define DAC_CLK_L	GPIOPinWrite(DAC_PORT, DAC_CLK, 0x00)

#define DAC_CS_H	GPIOPinWrite(DAC_PORT, DAC_CS, 0xff)
#define DAC_CS_L	GPIOPinWrite(DAC_PORT, DAC_CS, 0x00)

extern void DAC_init_gpio(void);
extern void DAC_write_data(unsigned short dat, int channel);

#endif /* __DAC_5618_H__ */
