/**
 * @file lab2.c
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
int main(int argc, char *argv[])
{ 
	int numprocs, myrank, namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// get current processor name
	MPI_Get_processor_name(processor_name, &namelen);

	// concurrently print the rank and name out of all processor
	printf("\tProcess %d on %s out of %d\n", myrank,
		processor_name, numprocs);

	// finalize MPI
	MPI_Finalize();
}

/* a. Why do the numbers in the sample hello.c come out of order? */
/**
	The process run concurrently.
  */

/* b. Name the two commands to initialize and finalize a MPI program. */
/* 
	MPI_Init(&argc, &argv);
	MPI_Finalize();	
 */
 
/* c. Explain the function of MPI_Comm_World. */
/**
	MPI_COMM_WORLD is a Communicator.
	All MPI communication calls require a communicator argument and 
	MPI processes can only communicate if they share a communicator.
  */

/* d. Describe the function of MPI_Comm_rank. */
/**
	MPI_COMM_RANK indicates the rank of the process that calls it in the range from size-1, 
	where size is the return value of MPI_COMM_SIZE.
  */