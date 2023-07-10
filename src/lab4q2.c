/**
 * @file lab4q2.c
 * @author Shazwan Bin Sharum Mizan
 * @brief Shazwan's Answer for Parallel Processing Lab
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <mpi.h>

#define BUFSIZE 256


int main(int argc, char **argv) {

    int myrank, numprocs, i;
    char buf[BUFSIZE];

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	////////////////////////////////////////////////////

	int nPartition;
	double result, localResult, start, end;

	// getting user input
	if (!myrank) {
		printf("Calculating Pi. Enter Partitions: ");
		fflush(stdout);
		scanf("%d", &nPartition);
	}

	// broadcasting the user input
	MPI_Bcast(&nPartition, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// start computation
	double step = 1.0/nPartition;
	double x, total=0;

	// the formula
	// notice that it start at myrank and skips by numprocs 
	/* 
		eg. numprocs 3
		processor 0: 0, 3, 6,  9, 12, ...
		processor 1: 1, 4, 7, 10, 13, ...
		processor 2: 2, 5, 8, 11, 14, ...
		until nPartition is reached
	*/
	for (i=myrank; i<nPartition; i+=numprocs) {
		x=(step/2) + step*i;
		total = total + 4/(1+x*x);
	}

	localResult = total*step;
	///////////////////////////////////////////////////////////////////////////////////////////////////

	// merging all the data by summation
	MPI_Reduce(&localResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// displaying the result
	printf("Rank %d: Pi is %1.20f\n", myrank, localResult);
	if (!myrank) {
		printf("Pi is %1.20f\n", result);
	}
	
	//////////////////////////////////////////////////////

	// finalize MPI	
    MPI_Finalize();
}