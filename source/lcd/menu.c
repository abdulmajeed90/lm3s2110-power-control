/*
 *  menu.c - 2013年07月18日 14时51分38秒
 *
 *  Description:  write menu on LCD
 * 
 *  Copyright (c) 2013, chenchacha
 */
#include <stdlib.h>
#include <stdio.h>
#include "lcd/display.h"
#include "periph/button.h"
#include "periph/infrared.h"
#include "wave.h"
#include "lcd/menu.h"

/* The page list of the menu */
MENU_LIST_t menu_list[FRAME_BUFFER_SCREEN];

unsigned char frame_buffer[FRAME_BUFFER_ROW_MAX][FRAME_BUFFER_COLUMN_MAX];

frame_buffer_t fb;
static unsigned int fb_place=0;
static int now_screen=0;
extern volatile unsigned int infrared_flag;
extern volatile unsigned int gpio_b_int_status;
unsigned char menu_operation;

/*  menu_roll - roll the display to screen
 *  return now screen.
 */
extern unsigned char spwm_step;
extern unsigned int spwm_voltage;
static int menu_roll(int screen)
{

	/* refresh screen */
	if (screen == now_screen) {
		fb_place = display_roll(&fb, fb_place, 0, 0, 0);
	} else if (screen > now_screen) {
		/* left roll the screen */
		fb_place = display_roll(&fb,fb_place,1,1, 1);
		fb_place = display_roll(&fb,fb_place,7,1, 7);

		fb_place = display_roll(&fb,fb_place,60,1, 20);

		fb_place = display_roll(&fb,fb_place,15,1, 5);
		fb_place = display_roll(&fb,fb_place,1,1, 1);
		now_screen++;
	} else if (screen < now_screen) {
		/* right roll the screen */
		fb_place = display_roll(&fb,fb_place,1,0, 1);
		fb_place = display_roll(&fb,fb_place,7,0, 7);

		fb_place = display_roll(&fb,fb_place,60,0, 20);

		fb_place = display_roll(&fb,fb_place,15,0, 5);
		fb_place = display_roll(&fb,fb_place,1,0, 1);
		now_screen--;
	}
	/* set pwm counter step */
	/* spwm_step = (now_screen+1) * 4; */
	/* 	wave_spwm_load(now_screen * 0x100 + 5800); */
	/* wave_spwm_data(now_screen+1); */

	return now_screen;
}		/* -----  end of function menu_roll  ----- */

/* menu_add_string - add string on framebuffer
 */
static void menu_add_string(int page, int num, const char *string)
{
	int x=0, y=0;

	/* the screen filled */
	y = num % 6;
	x = 84 * (page-1);

	display_add_string(&fb, x, y, string);
}		/* -----  end of function menu_add_string  ----- */

/* menu_title - add title on this page
 * @page: the range is 1 - FRAME_BUFFER_SCREEN.
 */
static void menu_title(int page, const char *title)
{
	int x = 84 * (page-1);

	display_add_string(&fb, x, 0, title);
}		/* -----  end of function menu_title  ----- */

/* menu_nagation_entry -
 */
static void menu_nagation_entry(int page, int num)
{
	int x = 84 * (page-1);
	int y = num * 8;

	display_negation_boxes(&fb, x, y-1, 83, 8);
}		/* -----  end of function menu_nagation_entry  ----- */

/* menu_clean_page - clean a specify page
 */
static void menu_clean_page(int page)
{
	unsigned int x = 84*(page-1);

	display_clean(&fb, x, 0, x+84, 48, 0);
}		/* -----  end of function menu_clean_page  ----- */

/* menu_input - input a date
 */
#define MENU_INPUT_STEP	(8)
static void menu_input(int page, unsigned int *dat, int num)
{
	unsigned char flags=0;
	int input;
	int size;
	char string[16];
	unsigned int value = *dat;
	int x=(page-1)*84;
	int y1=num*8 - 2;
	int y2=y1+10;

	/* start a input window */
	while (!(y1 <= 8 && y2 >= (48 - MENU_INPUT_STEP))) {
		if (y1 > 8)
			y1 -= MENU_INPUT_STEP;

		display_clean(&fb, x, y1, 83, MENU_INPUT_STEP, 1);
		display_clean(&fb, x, y2, 83, MENU_INPUT_STEP, 1);
		menu_roll(now_screen);

		if (y2 < (48 - MENU_INPUT_STEP))
			y2 += MENU_INPUT_STEP;
	}
	/* get the data size */
	for (size=0; value != 0; size++) {
		value /= 10;
	}
	value = *dat;

	while (flags != MENU_CONFIRM) {
		/* recieved infrared sign */
		if (infrared_flag == 1) {
			infrared_flag = 0;

			flags = infrared_value();
			/* if input number */
			switch (flags) {
				case MENU_0:input = 0;break;
				case MENU_1:input = 1;break;
				case MENU_2:input = 2;break;
				case MENU_3:input = 3;break;
				case MENU_4:input = 4;break;
				case MENU_5:input = 5;break;
				case MENU_6:input = 6;break;
				case MENU_7:input = 7;break;
				case MENU_8:input = 8;break;
				case MENU_9:input = 9;break;
				case MENU_BACKSPACE: /* if input backspace */
							if (size > 0) {
								size--;
								value /= 10;
							}
							continue;
				case MENU_CONFIRM: continue;
				default: continue;
			}
			size++;
			value *= 10;
			value += input;

		}
		sprintf(string, "%10d", value);
		menu_add_string(page, num, string);
		menu_roll(now_screen);
	}
	*dat = value;
}		/* -----  end of static function menu_input  ----- */

/* menu_display_parameter - display the parameter page
 */
static void menu_display_parameter(int page, void *para_v)
{
	char string[16];
	MENU_PARAMETER_t *para = (MENU_PARAMETER_t *)para_v;
	
	menu_title(page, "====Parameter===");
	/* display volage and current value */
	sprintf(string, "U=%d.%dV\nI=%d.%dA", para->voltage/1000, para->voltage%1000, para->current/1000, para->current%1000);
	menu_add_string(page, 1, string); 
	/* no operation */
	if (now_screen == page-1) 
		menu_operation = 0;

}		/* -----  end of static function menu_display_parameter  ----- */

/* menu_display_wave -
 */
#define MENU_WAVE_ENTRY_TOP		3
#define MENU_WAVE_ENTRY_BOTTOM	4
static void menu_display_wave(int page, void *wave_v)
{
	static unsigned int entry_place = MENU_WAVE_ENTRY_TOP;
	MENU_WAVE_t *wave = (MENU_WAVE_t *)wave_v;
	char string[32];
	unsigned int *value[MENU_WAVE_ENTRY_BOTTOM - MENU_WAVE_ENTRY_TOP + 1];

	value[0] = &wave->amplitude;
	value[1] = &wave->frequency;

	menu_title(page, "======wave======");
	/* display the period1 */
	sprintf(string, "P1=%d", wave->period1);
	menu_add_string(page, 1, string);
	/* display the period2 */
	sprintf(string, "P2=%d", wave->period2);
	menu_add_string(page, 2, string);
	/* display the parameter of waveform */
	sprintf(string, "A=%dV\nF=%dHz", *value[0], *value[1]);
	menu_add_string(page, 3, string);

	/* menu operation */
	if (now_screen == page-1) {
		switch (menu_operation) {
			case MENU_DOWN: /* select entry */
				if (entry_place < MENU_WAVE_ENTRY_BOTTOM) {
					menu_nagation_entry(page, entry_place++);
				}
				break;

			case MENU_UP:
				if (entry_place > MENU_WAVE_ENTRY_TOP) {
					menu_nagation_entry(page, entry_place--);
				}
				break;

			case MENU_INC: /* date increase and decrease */
				(*value[entry_place - MENU_WAVE_ENTRY_TOP])++;
				break;

			case MENU_DEC:
				(*value[entry_place - MENU_WAVE_ENTRY_TOP])--;
				break;
				
			case MENU_CONFIRM:
				menu_input(page, value[entry_place-MENU_WAVE_ENTRY_TOP], entry_place);
				break;

			default:
				break;
		} 
		menu_operation = 0;
		menu_nagation_entry(page, entry_place);
	}

}		/* -----  end of function menu_display_wave  ----- */

/* menu_display_empty -
 */
static void menu_display_empty(int page, void *para)
{
}		/* -----  end of function menu_display_empty  ----- */

/*  menu_start - initial all module for menu
 *  initial LCD.
 *  initial button.
 */
void menu_start(void)
{
	int i;
/* 
 * 	unsigned char *frame_buffer;
 * 	unsigned long lenght = FRAME_BUFFER_ROW_MAX*FRAME_BUFFER_COLUMN_MAX;
 *  	frame_buffer = (unsigned char *)malloc((sizeof (unsigned char)) * lenght);
 */

	/* initial LCD */
	fb.fb = (unsigned char *)frame_buffer;
	fb.column_max = FRAME_BUFFER_COLUMN_MAX;
	display_start(&fb);

	/* initial button */
	/* button_init_gpio(); */
	/* clean the menu list */
	for (i=1; i <= FRAME_BUFFER_SCREEN; i++)
		menu_list_clean(i, 0);
	
	/* infrared initialize */
	infrared_init();
}		/* -----  end of function menu_start  ----- */

void menu_end(void)
{
/* 	free(fb.fb); */
}

/*  menu_refresh - refresh the display with button
*/
int menu_refresh(void)
{
	static int screen = 0;
	unsigned char flags;

	/* recieved infrared sign */
	if (infrared_flag == 1) {
		infrared_flag = 0;

		flags = infrared_value();
		/* select button */
		switch (flags) {
			case MENU_FORWARD: /* forward page */
				if (screen > 0)
					screen--;
				break;
			case MENU_BACK:	/* back page */
				if (screen < (FRAME_BUFFER_SCREEN - 1))
					screen++;
				break;
			case MENU_UP: /* entry up */
			case MENU_DOWN: /* entry down */
			case MENU_INC: /* data increase */
			case MENU_DEC: /* date decrease */
			case MENU_CONFIRM:
				menu_operation = flags;
				break;
			case 0:
			default:
				break;
		}
	}
	menu_roll(screen);

	return screen;
}		/* -----  end of function menu_refresh  ----- */

/* menu_clean_now - clean now screen
 */
void menu_clean_now(void)
{
	display_clean(&fb, fb_place, 0, fb_place+84, 48, 0);
}		/* -----  end of function menu_clean_now  ----- */

/* menu_list_clean - clean the menu list
 */
void menu_list_clean(int page, void *para)
{
	menu_list[page-1].display = menu_display_empty;
	menu_list[page-1].para = para;
}		/* -----  end of function menu_list_clean  ----- */

/* menu_init_parameter - initialize the parameter page
 * Connect the parameter.
 */
void menu_init_parameter(int page, MENU_PARAMETER_t *para)
{
	menu_list[page-1].display = menu_display_parameter;
	menu_list[page-1].para = (void *)para;
}		/* -----  end of function menu_init_parameter  ----- */

/* menu_init_wave -
*/
void menu_init_wave(int page, MENU_WAVE_t *wave)
{
	menu_list[page-1].display = menu_display_wave;
	menu_list[page-1].para = (void *)wave;
}		/* -----  end of function menu_init_wave  ----- */

/* menu_display - display now screen
 * display now page an other page will stop.
 */
void menu_display(void)
{
	int screen;

	screen = menu_refresh();

	menu_clean_page(screen+1);
	/* Display now page */
	(*menu_list[screen].display)(screen+1, menu_list[screen].para);
	/* Display on LCD */
	menu_roll(screen);
}		/* -----  end of function menu_display  ----- */
