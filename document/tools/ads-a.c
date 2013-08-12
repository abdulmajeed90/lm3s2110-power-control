/*
 * ads-a.c - 2013年08月12日 15时30分17秒
 *
 * Copyright (c) 2013, chenchacha
 * 
 */

#include <stdlib.h>
#include <stdio.h>

#define ads_p1 1.537
/* #define ads_p1 1.526
 */
/* #define ads_p1 1.547
 */

#define ads_p2  20240

/* #define ads_p2  20430
 */
/* #define ads_p2  20050
 */


#define ads_value(x)	((double)((double)(x) * ads_p1 - ads_p2))

#define ads_num 11
double ads[] = {
	15219,15470,15820,16297,
	17186,18051,19039,19700,
	20373,21387,22213,
};

/* 
 *  main
 *  Description:  
 */
int main(int argc, char *argv[])
{
	int i=0;
	double tmp;

	for (i=0; i<ads_num; i++) {
		tmp = ads_value(ads[i]);
		printf("%lf,", tmp);
	}

	printf("\n");
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
