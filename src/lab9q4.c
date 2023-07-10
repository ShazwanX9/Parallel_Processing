/**
 * @file lab9q4.c
 * @author Shazwan Bin Sharum Mizan
 * @brief Shazwan's Answer for Parallel Processing Lab
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <math.h>
#include <omp.h>

#define START (M_PI/2)
#define END (M_PI*2)

double f(double x)
{
    return sin(x/2)+1;
}

int main(int argc, char *argv[])
{ 
	double total = 0, x;
	int partitions;
	double slice;

	printf("How many partitions? "); fflush(stdout);
	scanf("%d", &partitions);

	slice = (END - START)/partitions;

	
	for (x = START + (slice/2); x < END; x = x + slice)
		total = total + f(x);
	total = total * slice;
	printf("The SERIAL integration is %1.10f\n", total);
/* 
	// P/S: Cant even run it hahah
	total = 0; // reset total
	#pragma omp parallel for 
	for (x = START + (slice/2); x < END; x = x + slice)
		total = total + f(x);
	total = total * slice;
	printf("The FAIL PRAGMA OMP integration is %1.10f\n", total);
 */
 
	// Solution: avoid it altogether by looping over 
	// the partitions with an integer variable
	total = 0; // reset total
	#pragma omp parallel for private(x) reduction(+:total)
	for(int i = 0; i < partitions; i++) {
	  x = START + slice/2 + i*slice;
	  total = total + f(x);
	}
	total = total * slice;
	printf("The SUCCEED PRAGMA OMP integration is %1.10f\n", total);
}