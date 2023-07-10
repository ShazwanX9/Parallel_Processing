/**
 * @file lab7q2.c
 * @author Shazwan Bin Sharum Mizan
 * @brief Shazwan's Answer for Parallel Processing Lab
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define BUFSIZE 256

int main(int argc, char **argv) {
    int myrank, numprocs;
	char filename[BUFSIZE];
	FILE *outfile;
	int size;

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// in question; just carried out on top
	srandom(time(0));


	/* 
		Isolate the parts that talk to the user (both input and output) 
		and restrict those to be done only by process 0.
	*/
	if (!myrank) {
		// printf("What is the file name you want to write to? ");
		// fflush(stdout);
		// scanf("%s", filename);
		// printf("How many long integers to generate? ");
		// fflush(stdout);
		// scanf("%d", &size);


		printf("Like Usual, I'll Just Use Dummy Data Lolz...\n");
		sprintf(filename, "lab7q2_result.txt");
		size = 100;
	}

	/* 
		Broadcast the information received from the user 
		that other processes need to know to compute 
		their part to all the other processes.
	*/
	MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int local_size = size/numprocs;
	long int* local_data = (long int *)malloc(sizeof(long int)*local_size);

	int covered_size = local_size*numprocs;
	long int* covered_data = (long int *)malloc(sizeof(long int)*size);

	/*
		1. You will need to generate the random numbers into an array
	*/
	for (int i=0; i<local_size; i++)
		local_data[i] = random();

	/*
		2. send them back to process 0 to write to the file
	*/
	MPI_Gather(
		local_data, local_size, MPI_LONG,
		covered_data, local_size, MPI_LONG,
		0, MPI_COMM_WORLD
	);

	if (!myrank) {
		outfile = fopen(filename,"w");
		if (!outfile)
		{ 
			fprintf(stderr,"Unable to open %s for writing.\n", filename);
			exit(1);
		}

		
		fprintf(outfile,"%d\n",size);

		for (int i=0; i<covered_size; i++)
			fprintf(outfile,"%ld\n", covered_data[i]);

		/*
			3. You will also need to have process 0 
			take care of any left-overs in case the 
			number of long integers to generate does 
			not divide exactly by the number of processes
		*/
		for (int i=covered_size; i<size; i++)
			fprintf(outfile,"%ld\n", random());
			
		fclose(outfile);
	}



	// finalize MPI	
    MPI_Finalize();
}