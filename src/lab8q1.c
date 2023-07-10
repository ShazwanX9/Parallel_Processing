/**
 * @file lab8q1.c
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
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BUFSIZE 256

double f(double x){
	return (sin (x/2) +1);
}

int main(int argc, char **argv) {

	// variable declaration
	int myrank, numprocs;
	double a,b,w,sum=0,y[2];
	int n;
	double btime;

	// STARTER TEMPLATE: no need to know detail
	// it is what it is
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	////////////////////////////////////////////////////

	// !myrank also mean myrank == 0
	if (!myrank) {

		printf("Enter the number of intervals : "); fflush(stdout);
		scanf("%d", &n);

		// Formula from the question
		a=M_PI/2;
		b=2*M_PI;
		//step length (width of each trapezoid)
		w=(b-a)/n;
		
		//set start time
		btime = MPI_Wtime();
	}

	// broadcast number of partition to all rank
	// one could say we global the &nPartition value from user receive at rank 0
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&w, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// locally compute the value for respective 

	// Initialize the heights
	int range = n/numprocs;
	int startIndex 	= range*myrank;
	int endIndex 	= startIndex+range;

	// y[0] = f(a) -> y[startIndex%2] = f(a+startIndex*w);
	y[startIndex%2] = f(a+startIndex*w);	
	printf("%d: %d -> %d\n", myrank, startIndex, endIndex-1);

	// covered by chunk
	for(int i=startIndex+1; i<=endIndex; i++) {
		y[i%2] = f(a+i*w);
		sum += y[i%2] + y[(i+1)%2];
	}

	double localResult = sum*w/2;
	printf("%d: localResult %1.20f\n", myrank, localResult);

	// merging all the data
	// one could say &result is global result that sum all the &localResult
	double result;
	MPI_Reduce(&localResult, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	// !myrank also mean myrank == 0
	if (!myrank) {

		// compute the remainder
		sum = 0; // reset the sum

		int remainderIndex = n-n%numprocs;
		y[remainderIndex%2]= f(a+remainderIndex*w);
		for (int i=remainderIndex+1; i<=n; i++) {
			y[i%2] = f(a+i*w);
			sum += y[i%2] + y[(i+1)%2];
		}
		result += sum*w/2;

		printf("0: %d -> %d\n", remainderIndex, n);
		printf("0: remainder localResult %1.20f\n", sum*w/2);
		printf("\n");
		printf("Results: %1.20f\n", result);
		printf("Parallel time: %1.20f\n", MPI_Wtime() - btime);
		printf("\n");
	}

	//////////////////////////////////////////////////////

	// STARTER TEMPLATE: no need to know detail
	// it is what it is
    MPI_Finalize();
}
