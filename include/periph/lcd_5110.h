#ifndef __LCD_5110_H__
#define __LCD_5110_H__

#define LCD_PERIPH	SYSCTL_PERIPH_GPIOA
#define LCD_PORT	GPIO_PORTA_BASE

#define LCD_CE		GPIO_PIN_0
#define LCD_RST		GPIO_PIN_1
#define LCD_DC		GPIO_PIN_2
#define LCD_DAT		GPIO_PIN_3
#define LCD_CLK		GPIO_PIN_4
#define LCD_PORT_MASK	(LCD_CE | LCD_RST | LCD_DC | \
			LCD_DAT | LCD_CLK)

#define LCD_RST_H	GPIOPinWrite(LCD_PORT, LCD_RST, 0xff)
#define LCD_RST_L	GPIOPinWrite(LCD_PORT, LCD_RST, 0x00)

#define LCD_CE_H	GPIOPinWrite(LCD_PORT, LCD_CE, 0xff)
#define LCD_CE_L	GPIOPinWrite(LCD_PORT, LCD_CE, 0x00)

#define LCD_DC_H	GPIOPinWrite(LCD_PORT, LCD_DC, 0xff)
#define LCD_DC_L	GPIOPinWrite(LCD_PORT, LCD_DC, 0x00)

#define LCD_DAT_H	GPIOPinWrite(LCD_PORT, LCD_DAT, 0xff)
#define LCD_DAT_L	GPIOPinWrite(LCD_PORT, LCD_DAT, 0x00)

#define LCD_CLK_H	GPIOPinWrite(LCD_PORT, LCD_CLK, 0xff)
#define LCD_CLK_L	GPIOPinWrite(LCD_PORT, LCD_CLK, 0x00)

extern void LCD_clean(void);
extern void LCD_init(void);
extern void LCD_set_pos(unsigned int x, unsigned int y);
extern void LCD_draw_frame_buffer(int start_row, int start_column,
								unsigned char *frame_buffer, int column_max);
extern void LCD_start();
extern void LCD_draw_point(int row, int column);
extern void LCD_draw_dollop(int startx, int starty, int endx, int endy);
#endif /* __LCD_5110_h__ */
