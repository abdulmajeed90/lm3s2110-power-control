#include "hw_memmap.h"
#include "hw_types.h"
#include "src/ssi.h"
#include "src/gpio.h"
#include "src/sysctl.h"
#include "src/systick.h"

/* SSI_init_spi_16() - initialize SSI and configure to 16 bit SPI
 */
void SSI_init_spi_16(void)
{
	/* enable SSI module */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI);
	/* enable GPIOA */
	SysCtlPeripheralEnable(SSI0CLK_PERIPH);
	/* enable GPIOB */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	/* configure SSI master and 16 bit, 115200 p */
	GPIOConfig(SSI_BASE, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, \
			BitRate, DataWidth);

}
