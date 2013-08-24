/*
 * pid.c - 2013年08月14日 02时10分04秒
 *
 * Copyright (c) 2013, chenchacha
 *
 * The Algorithm PID. The proportion, integral and derivative.
 * P: u(t)=Kp*e(t);
 * PI: u(t)=Kp*e(t)+1/Ti*(int(e(t),t,t,0));
 * PID: u(t)=Kp*e(t)+1/Ti*(int(e(t),t,t0))+Td*diff(e(t),'t',1);
 */
#include "calculation/pid.h"

/* pid_calc - get the change value by pid
 */
long pid_calc(PID_t *pid)
{
	pid->perror = (long)(pid->target - pid->value);
	pid->ierror += pid->perror;
	pid->derror = pid->last_error - pid->prev_error;
	pid->prev_error = pid->last_error;
	pid->last_error = pid->perror;

	return (pid->kp * pid->perror +
			pid->ti * pid->ierror +
			pid->td * pid->derror);
}		/* -----  end of function pid_calc  ----- */
