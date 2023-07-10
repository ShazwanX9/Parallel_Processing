/**
 * @file lab3q2e.c
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

/**
	MPI_Send & MPI_Recv VS MPI_Bcast

	MPI_Bcast(
		void* data,
		int count,
		MPI_Datatype datatype,
		int root,
		MPI_Comm communicator
	)

	-> data need to specify count and datatype
	-> data will be shared to other processor based on MPI_Comm
	-> data that not root would be overwritten
	-> root would be sender and other will be receiver

	NOTE: will block until the message has been transmitted
*/

int main(int argc, char **argv) {
    int myrank, numprocs, value;

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Using MPI_Send & MPI_Recv
	if (myrank == 0) {
		for (int i=0; i<numprocs; i++) {
			if (i != myrank) {
				value = 1;
				// processor 0 send
				MPI_Send(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}
	}

	else {
		value = -1;
		printf("Value on processor %d before receiving: %d\n", myrank, value);

		// other processor receive
		MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Value on processor %d after receiving: %d\n", myrank, value);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	// Using 2 MPI_Bcast
	if (myrank ==0) {
		value = 2;
		// processor 0 send
		MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} 
	else {
		value = -2;
		printf("Value on processor %d before receiving: %d\n", myrank, value);
		
		// other processor receive
		MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Value on processor %d after receiving: %d\n", myrank, value);		
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	// Using 1 MPI_Bcast
	if (myrank ==0) {
		value = 3;
	}
	else {
		value = -3;
		printf("Value on processor %d before receiving: %d\n", myrank, value);
	}
	// all processor calling bcast together [0 send and other recieve]
	MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Value on processor %d after receiving: %d\n", myrank, value);		

	///////////////////////////////////////////////////////////////////////////////////////////////////

	// finalize MPI	
    MPI_Finalize();
    return 0;
}