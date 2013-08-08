/*
 *  frame-buffer.h - 2013年07月14日 18时07分03秒
 *
 *  Description:  
 * 
 *  Copyright (c) 2013, chenchacha
 */
#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

extern void fb_write_char(int row, int column, const char *ptrc, unsigned char *frame_buffer, int column_max);
extern void fb_write_point(int row, int column, unsigned char *fb, int column_max);
extern void fb_write_line(int start_r, int start_c, int length, int width, unsigned char *fb, int column_max);
extern void fb_write_dollop(int starx, int starty, int endx, int endy, unsigned char *fb, int column_max);
extern void fb_negation_point(int row, int column, unsigned char *fb, int column_max);
extern void fb_negation_dollop(int startx, int starty, int endx, int endy, unsigned char *fb, int column_max);
#endif /* __FRAME_BUFFER_H__ */
