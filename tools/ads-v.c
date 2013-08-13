/*
 * ads-v.c - 2013年08月12日 17时25分12秒
 *
 * Copyright (c) 2013, chenchacha
 */

#include <stdlib.h>
#include <stdio.h>

#define ads_v1	1.384
#define ads_v2	3.055
#define ads_voltage_value(x)	((x)*ads_v1 + ads_v2)

#define ads_v_num	9
double ads_v_array[] = {
	2090,3510,5717,
	6832,8848,11514,
	13968,17290,22860,
};

/* 
 *  main
 *  Description:  
 */
int main(int argc, char *argv[])
{
	int i;

	for (i=0; i<ads_v_num; i++) {
		printf("%lf,", ads_voltage_value(ads_v_array[i]));
	}
	printf("\n");

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
