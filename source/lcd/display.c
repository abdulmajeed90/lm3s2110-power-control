/*
 *  display.c - 2013年07月14日 18时31分51秒
 *
 *  Description:  display on LCD
 * 
 *  Copyright (c) 2013, chenchacha
 */
#include <stdio.h>
#include <string.h>
#include "periph/lcd_5110.h"
#include "lcd/frame-buffer.h"
#include "lcd/display.h"

char title[100];

/*  display_start - start display on LCD
*/
void display_start(frame_buffer_t *frame_buffer)
{
	unsigned char *fb = frame_buffer->fb;
	int column_max = frame_buffer->column_max;

	LCD_init();

	fb_write_char(0,5,"NOKIA 5110", fb, column_max);
 	sprintf(title, "Description: chenchachan - Copyright (c) 2013, chenchacha");
 	fb_write_char(1,0,title, fb, column_max); 

/* 	fb_negation_dollop(0,0,83,47,fb,column_max);
 */
/* 	display_boxes(frame_buffer); */ 

	LCD_draw_frame_buffer(0,0,fb, column_max);

	LCD_draw_dollop(0,0,83,47);
#if 0
	LCD_draw_point(30,25);
	/* write a boxes */
	LCD_draw_dollop(25,48,0,0);
	LCD_draw_dollop(25,25,0,84);
	LCD_draw_dollop(48,48,0,84);
	LCD_draw_dollop(25,48,84,84);
#endif
}		/* -----  end of function display_start  ----- */

/*  display_roll - roll the framebuffer
 *  place: place point in framebuffer.
 *  roll: roll scope.
 *  dirctrion: roll dirction.
 */
extern volatile int button_count;
int display_roll(frame_buffer_t *frame_buffer, int place, int roll, int dirction, int step)
{
	unsigned char *fb = frame_buffer->fb;
	int column_max = frame_buffer->column_max;
	int i = place;

	/* display button interrupt count */
	sprintf(title, "button count:%d", button_count);
	fb_write_char(5,0,title, fb, column_max); 

	/* roll left */
	if (dirction) {
		while (i<place+roll) {
/* 			sprintf(title, "%d,fixed position roll left:%d", i, i);
 * 			fb_write_char(3,0,title, fb, column_max); 
 */

			LCD_draw_frame_buffer(0,i,fb,column_max);
			i += step;
		}
	} else {  /* roll right */
		while (i>place-roll) {
/* 			sprintf(title, "%d,fixed position roll right:%d", i, i);
 * 			fb_write_char(4,0,title, fb, column_max); 
 */

			LCD_draw_frame_buffer(0,i,fb,column_max);
			i -= step;
		}
	}
	LCD_draw_frame_buffer(0,i,fb,column_max);

	return i;
}		/* -----  end of function display_roll  ----- */

/*  display_boxes - write a boxes on LCD
 */
void display_boxes(frame_buffer_t *frame_buffer,int x, int y, int length, int width)
{
	unsigned char *fb = frame_buffer->fb;
	int column_max = frame_buffer->column_max;

	fb_write_dollop(x,y,x+length,y+width,fb,column_max);
	fb_negation_dollop(x+1,x+y,x+length-1,y+width-1,fb,column_max);
}		/* -----  end of function display_boxes  ----- */

/* display_add_string - add a string on LCD
 */
void display_add_string(frame_buffer_t *frame_buffer, int x, int y, const char *string)
{
	unsigned char *fb = frame_buffer->fb;
	int column_max = frame_buffer->column_max;

	fb_write_char(x,y,string, fb, column_max); 

}		/* -----  end of function display_add_string  ----- */
