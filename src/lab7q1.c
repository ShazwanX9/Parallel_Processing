/**
 * @file lab7q1.c
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
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256

int main(int argc, char **argv) {
	int myrank, numprocs;

	// variable declaration
	int *data, *localData;
	int size, i;

	FILE *infile;
	char buf[BUFSIZE];

	int result, localResult;
	double start, end;
	double btime;

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	////////////////////////////////////////////////////

	// !myrank also mean myrank == 0
	if (!myrank) {
		// Requesting user input
		/*
	  	printf("What is the name of the data file?\n");
		fgets(buf,BUFSIZE,stdin);
		buf[strlen(buf)-1]='\0'; // Remove the carriage return from the file name

		infile = fopen(buf,"r");
		*/

		printf("I'll just use the dummy `%s` here lolz.\n", "lab07dat1.txt");
		infile = fopen("lab07dat1.txt","r");
		if (infile==NULL) { 
			perror ("Opening file");
			exit(1);
		}

		fscanf(infile,"%d", &size); // Find out how big the data is
		printf("Size = %d\n",size);

		data = (int *)malloc(sizeof(int)*size); // Allocate the space
		for (int i=0; i<size; i++) // Read the data
		{ 
			fscanf(infile,"%d",&data[i]);
			/* printf("%d ",data[i]); */
		}
		
		// reset time before execution for parallel
		btime = MPI_Wtime();
	}

	// broadcast number of partition to all rank
	// one could say we global the &nPartition value from user receive at rank 0
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// size without additional
	int totalSize = size-(size%numprocs);
	int localSize = totalSize/numprocs;

	localData = (int *)malloc(sizeof(int)*(localSize)); // Allocate the space
	MPI_Scatter(data, localSize, MPI_INT, localData, localSize, MPI_INT, 0, MPI_COMM_WORLD);

	// locally compute the value for respective 
	localResult = 0;
	for (int i=0; i<localSize; i++) {
		localResult += localData[i];
	}

	// merging all the data
	// one could say &result is global result that sum all the &localResult
	MPI_Reduce(&localResult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	// !myrank also mean myrank == 0
	if (!myrank) {
		printf("\n");
		printf("Total Result without remaining: %d\n", result);
		
		for (int i=totalSize; i<size; i++)
			result += data[i];
		printf("Total Result: %d\n", result);
		
		printf("Parallel time: %1.20f\n", MPI_Wtime() - btime);
		printf("\n");
	}

	//////////////////////////////////////////////////////

	// STARTER TEMPLATE: no need to know detail
	// it is what it is
    MPI_Finalize();
}
