/**
 * @file lab5q2.c
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

    int my_rank, numprocs, i;
    char buf[BUFSIZE];
	double btime;

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	////////////////////////////////////////////////////

	int nPartition;
	double result, localResult, start, end;

	double step;
	double x, total=0;

	// getting user input
	if (!my_rank) {
		printf("Calculating Pi. Enter Partitions: ");
		fflush(stdout);
		scanf("%d", &nPartition);

		// time before execution for serial
		btime =  MPI_Wtime();

		// overall different is this line
		// so we calculate each and every step 
		// from 0 till number of partition reach
		step = 1.0/nPartition;
		for (i=0; i<nPartition; i++) {
			x=(step/2) + step*i;
			total = total + 4/(1+x*x);
		}
		double serialResult = total*step;

		printf("Serial result for Pi is %1.20f\n", serialResult);
		printf("Serial time: %1.20f\n", MPI_Wtime() - btime);

		// time before execution for parallel
		btime =  MPI_Wtime();
	}

	// broadcasting the user input
	MPI_Bcast(&nPartition, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// start computation
	step = 1.0/nPartition;
	x, total=0;

	// the formula
	// notice that it start at my_rank and skips by numprocs 
	/* 
		eg. numprocs 3
		processor 0: 0, 3, 6,  9, 12, ...
		processor 1: 1, 4, 7, 10, 13, ...
		processor 2: 2, 5, 8, 11, 14, ...
		until nPartition is reached
	*/
	for (i=my_rank; i<nPartition; i+=numprocs) {
		x=(step/2) + step*i;
		total = total + 4/(1+x*x);
	}

	localResult = total*step;
	///////////////////////////////////////////////////////////////////////////////////////////////////

	// merging all the data by summation
	MPI_Reduce(&localResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// displaying the result
	printf("Rank %d: Pi is %1.20f\n", my_rank, localResult);
	if (!my_rank) {
		printf("Parallel result for Pi is %1.20f\n", result);
		printf("Parallel time: %1.20f\n", MPI_Wtime() - btime);
	}
	
	//////////////////////////////////////////////////////

	// finalize MPI	
    MPI_Finalize();
}