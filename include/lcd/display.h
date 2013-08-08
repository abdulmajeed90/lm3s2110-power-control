/*
 *  display.h - 2013年07月14日 18时32分07秒
 *
 *  Description:  display on LCD
 * 
 *  Copyright (c) 2013, chenchacha
 */
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define FRAME_BUFFER_SCREEN		(4)
#define FRAME_BUFFER_ROW_MAX	(6)
/* #define FRAME_BUFFER_COLUMN_MAX	(17*5*2) */
#define FRAME_BUFFER_COLUMN_MAX	(84 * FRAME_BUFFER_SCREEN)

typedef struct {
	unsigned char *fb;
	int column_max;
} frame_buffer_t;

extern void display_start(frame_buffer_t *frame_buffer);
extern int display_roll(frame_buffer_t *frame_buffer, int place, int roll, int dirction, int step);
extern void display_boxes(frame_buffer_t *frame_buffer,int x, int y, int length, int width);
extern void display_add_string(frame_buffer_t *frame_buffer, int x, int y, const char *string);
#endif /* __DISPLAY_H__ */
