/**
 * @file lab9q1.c
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

int main() {
	int i;
	printf("Hello World \n");

	printf("Total Thread That You Can Use -> %d \n", omp_get_max_threads());

	#pragma omp parallel for
	for(i=0;i<6;i++) {
		printf("\tIter:%d -> %d \n", i, omp_get_thread_num());
	}
	printf("GoodBye World \n");
}