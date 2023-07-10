/**
 * @file lab9q2.c
 * @author Shazwan Bin Sharum Mizan
 * @brief Shazwan's Answer for Parallel Processing Lab
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <omp.h>

float dot_prod(float* a, float* b, int N)
{
    float sum = 0.0;

	/*
		Keep in mind, everything is 
		not mentioned as private by default is shared
	*/
	// #pragma omp parallel for shared(sum) reduction(+:sum)
    #pragma omp parallel for reduction(+:sum)
    for(int i=0; i<N; i++) {
        sum += a[i] * b[i];
    }

    return sum;
}

int main() {
    int N = 10;  // Choose the length of vectors
    float a[N], b[N];

    // Fill vectors a and b for testing
	#pragma omp parallel for
    for(int i=0; i<N; i++) {
        a[i] = i;
        b[i] = i*2;
    }

    float result = dot_prod(a, b, N);
	
	
	/*
		Keep in mind: parallel the printing 
		will mess up your output
	*/

	printf("Data in A: [");
	for (int i=0; i<N; i++) {
		printf(" %.2f ", a[i]);
	}
	printf("]\n");

	printf("Data in B: [");
	for (int i=0; i<N; i++) {
		printf(" %.2f ", b[i]);
	}
	printf("]\n");


    printf("Dot product: %.2f\n", result);

    return 0;
}
