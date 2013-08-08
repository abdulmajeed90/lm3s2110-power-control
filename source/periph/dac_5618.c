/* driver for tlv5618
 */
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "src/debug.h"
#include "src/sysctl.h"
#include "src/gpio.h"
#include "periph/dac_5618.h"

#define DA_0832		1

/* DAC_init_gpio() - initialize DAC gpio
 */
void DAC_init_gpio(void)
{
	/* enable GPIO */
	SysCtlPeripheralEnable(DAC_PERIPH);
	/* configure GPIO in output */
	GPIOPinTypeGPIOOutput(DAC_PORT, DAC_PORT_MASK);

#if DA_0832
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 0xff);
#endif
}

/* DAC_write_data(dat,channel) - write 12 bit to DAC by SPI
 * @dat: write data by SPI.
 * @channel: select channel between A or B, 0 is A, 1 is B.
 */
/* #define DAC_A_HEAD	(0xc) */
#define DAC_A_HEAD	(0x8)
/* #define DAC_B_HEAD	(0x4) */
#define DAC_B_HEAD	(0x0)
void DAC_write_data(unsigned short dat, int channel)
{
	unsigned int i;

#if DA_0832
	GPIOPinWrite(GPIO_PORTF_BASE, 0xff, dat);
#endif

	/* enable DAC */
	DAC_CS_L;

	dat &= 0xfff;

	switch (channel) {
		case 0:
			dat |= (DAC_A_HEAD << 12);
			break;
		case 1:
			dat |= (DAC_B_HEAD << 12);
			break;
		default:
			break;
	}

	/* write 16 bit by GPIO */
	for (i = 0; i < 16; i++) {
		DAC_CLK_L;

		if (dat & 0x8000) DAC_DAT_H;
		else DAC_DAT_L;

		dat <<= 1;
		DAC_CLK_H;
	}
	DAC_CS_H;
}

