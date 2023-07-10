/**
 * @file lab3q1.c
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

	/**
		1. Instead of every process printing, they need to send the message to process 0 to print it, 
		since only process 0 is guaranteed to be on the computer that the user is at.
	*/
	// assign the string of rank and name of own processor to own `buf`
    sprintf(buf, "Testing process from %d of %d", myrank, numprocs);

	// send the `buf` string to processor 0 with tag id 0
    MPI_Send(buf, BUFSIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

	///////////////////////////////////////////////////////////////////////////////////////////////////

	// on process 0
    if (myrank == 0) {
		/**
			2. Process 0 then needs to receive the message and print them.
			4. Remember that process 0 needs to receive a whole bunch of messages, so you will
			need a loop in process 0 to do the receiving.
		*/
		// loop through all the processor
        for (i = 1; i < numprocs; i++) {
			/**
				5. The first way that most students do this, 
				you will notice that it always then prints out the results in order. 
				Ask me once you have reached this point for what to do to print
				out in whatever order it happens to come in.
			*/
			// // recover all the `buf` string from source i with tag id 0
            // MPI_Recv(buf, BUFSIZE, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

			// recover all the `buf` string from any source with tag id 0
            MPI_Recv(buf, BUFSIZE, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", buf);
        }
		///////////////////////////////////////////////////////////////////////////////////////////////////
    }

	// finalize MPI	
    MPI_Finalize();
}

/* 3. Google MPI_Send and MPI_Recv for the details. */
/**
	MPI_Send(
		void* data,
		int count,
		MPI_Datatype datatype,
		int destination,
		int tag,
		MPI_Comm communicator
	)

	-> data need to specify count and datatype
	-> data will be send to targeted destination and tag

	NOTE: will block until the message has been transmitted

	MPI_Recv(
		void* data,
		int count,
		MPI_Datatype datatype,
		int source,
		int tag,
		MPI_Comm communicator,
		MPI_Status* status
	)

	-> data need to specify count and datatype
	-> data will be receive from targeted source and tag

	NOTE: will block until the message has been transmitted
  */

/* 6. As always, please ask me for help if you have problems. Do not sit there being confused. I am here to help you. */
/* ... */