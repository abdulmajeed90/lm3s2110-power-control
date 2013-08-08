/* driver for nokia-5110 lcd
 * 2012.7.23
 */
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "src/debug.h"
#include "src/sysctl.h"
#include "src/gpio.h"
#include "periph/lcd_5110.h"

#define delay_ms(a)	SysCtlDelay((a) * (SysCtlClockGet() / 3000));
#define delay_us(a)	SysCtlDelay((a) * (SysCtlClockGet() / 3));


/* LCD_init_gpio() - initialize LCD gpio
 */
static void LCD_init_gpio(void)
{
	/* enable GPIO */
	SysCtlPeripheralEnable(LCD_PERIPH);
	/* configure GPIO in output */
	GPIOPinTypeGPIOOutput(LCD_PORT, LCD_PORT_MASK);
}

/* LCD_write_byte(dat,cmd) - write 8 bit to LCD by SPI
 * @dat: write data by SPI.
 * @cmd: flag of command or data, 0 is command, 1 is data.
 */
static void LCD_write_byte(unsigned char dat, unsigned int cmd)
{
	unsigned int i;
	
	/* enable LCD */
	LCD_CE_L;
	/* check command or data */
	if (cmd) LCD_DC_H;	/* dat is data */
	else LCD_DC_L;	/* dat is command */

	/* write data by GPIO */
	for (i = 0; i < 8; i++) {
		LCD_CLK_L;

		if (dat & 0x80) LCD_DAT_H;
		else LCD_DAT_L;

		dat <<= 1;
		LCD_CLK_H;
	}

	/* disable LCD */
	LCD_CE_H;
}

/* LCD_clean() - clean LCD display
 */
void LCD_clean(void)
{
	unsigned int i;

	LCD_write_byte(0x40, 0); /* start low */
	LCD_write_byte(0x80, 0); /* start line */

	for (i = 0; i < 504; i++)
		LCD_write_byte(0, 1);
}

/* LCD_init() - initialize LCD
 */
void LCD_init(void)
{
	LCD_init_gpio();

	/* reset the LCD */
	LCD_RST_L;
	delay_us(1);
	LCD_RST_H;

	/* enable the LCD */
	LCD_CE_L;
	delay_us(1);
	LCD_CE_H;
	delay_us(1);

	/* write initialize command */
	LCD_write_byte(0x21, 0);
	LCD_write_byte(0xc8, 0);
	LCD_write_byte(0x13, 0);
	LCD_write_byte(0x06, 0);
	LCD_write_byte(0x20, 0);
	LCD_write_byte(0x0c, 0);

	LCD_clean();
}

/* LCD_set_pos(x,y) - set pos in LCD
 * @x:
 * @y:
 */
void LCD_set_pos(unsigned int x, unsigned int y)
{
	LCD_write_byte(0x40 | y, 0);
	LCD_write_byte(0x80 | x, 0);
}

/* LCD_draw() - draw frame buffer on LCD
 */
void LCD_draw_frame_buffer(int start_row, int start_column,
							unsigned char *frame_buffer, int column_max)
{
	int i, column, row;
	int row_max = (start_row + 6 ) * column_max;

	LCD_set_pos(0, 0);
	/* enable LCD */
	LCD_CE_L;
	/* dat is data */
	LCD_DC_H;


	for (row=start_row*column_max; /* row in data of array */
			row<row_max;
			row += column_max) {
		for (column=start_column; column<start_column+84; column++) {
			for (i = 0; i < 8; i++) {
				LCD_CLK_L; 

				if ((*(frame_buffer+row+column)) & 0x80 >> i)
					LCD_DAT_H;
				else
					LCD_DAT_L;

				LCD_CLK_H;
			}
		}
	}

	/* disable LCD */
	LCD_CE_H;
}

/*  LCD_draw_point - write a point on LCD
*/
void LCD_draw_point(int row, int column)
{
	/* fixed position */
	LCD_set_pos(row, column);

	/* draw a point */
	LCD_CLK_L;
	LCD_DAT_H;
	LCD_CLK_H;
}		/* -----  end of function LCD_draw_point  ----- */


/*  LCD_draw_line - write a line on LCD
*/
void LCD_draw_dollop(int startx, int starty, int endx, int endy)
{
	int i, j;

	for (i=starty; i<=endy; i++)
		for (j=startx; j<=endx; j++) {
			/* enable LCD */
			LCD_CE_L;
			/* dat is data */
			LCD_DC_H;

			LCD_draw_point(i,j);
			/* disable LCD */
			LCD_CE_H;
		}
}		/* -----  end of function LCD_draw_line  ----- */
