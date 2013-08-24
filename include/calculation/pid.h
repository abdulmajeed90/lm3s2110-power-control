/*
 * pid.h - 2013年08月14日 13时54分32秒
 *
 * Copyright (c) 2013, chenchacha
 *
 */
#ifndef __PID_H__
#define __PID_H__

typedef struct {
	int kp;
	int ti;
	int td;
	unsigned long target;
	unsigned long value;
	long perror;
	long ierror;
	long derror;
	long last_error;
	long prev_error;
} PID_t;

extern long pid_calc(PID_t *pid);

#endif /* __PID_H__ */
