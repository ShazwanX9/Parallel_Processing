/**
 * @file lab3q2.c
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

/**
	MPI_Bcast is equivalent to these [can refer lab3q2e.c]

	if (myrank == root) {
		for (int i=0; i<numprocs; i++) {
			if (i != myrank) {
				MPI_Send(data, count, datatype, i, 0, communicator);
			}
		}
	}

	else {
		MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
	}


	NOTE: all processor must call the MPI_Bcast to prevent deadlock
*/



int main(int argc, char **argv) {
    int myrank, numprocs, value;

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// on processor 0
    if (myrank == 0) {
		// infinite loop
        while (1) {	
			// user prompt
            printf("Enter a non-negative integer (or a negative integer to quit): ");
            fflush(stdout);
            scanf("%d", &value);

			// check terminate condition
            if (value < 0) break;

			// sending `value` information to all other processor `value`
            MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

	// on other processor
	else {
		// infinite loop
        while (1) {
			// recv `value` information from processor 0
            MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

			// check terminate condition
            if (value < 0) break;

			// display from all other processor
            printf("Process %d received value: %d\n", myrank, value);
        }
    }

	// finalize MPI	
    MPI_Finalize();
    return 0;
}