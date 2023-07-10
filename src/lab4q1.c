/**
 * @file lab4q1.c
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

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int x, y, z;

	// case ( n == my_rank )
	switch (my_rank){
		case 0:
			x=0; y=1; z=2;
			MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
			printf("\tcase 0: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			MPI_Send(&y, 1, MPI_INT, 2, 43, MPI_COMM_WORLD);
			printf("\tcase 0: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			MPI_Bcast(&z, 1, MPI_INT, 1, MPI_COMM_WORLD);
			printf("\tcase 0: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			break;

		case 1:
			x=3; y=4; z=5;

			MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
			printf("\tcase 1: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			MPI_Bcast(&y, 1, MPI_INT, 1, MPI_COMM_WORLD);
			printf("\tcase 1: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			break;

		case 2:
			x=6; y=7; z=8;

			MPI_Bcast(&z, 1, MPI_INT, 0, MPI_COMM_WORLD);
			printf("\tcase 2: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			MPI_Recv(&x, 1, MPI_INT, 0, 43, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("\tcase 2: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			MPI_Bcast(&y, 1, MPI_INT, 1, MPI_COMM_WORLD);
			printf("\tcase 2: %d: x=%d y=%d z=%d\n", my_rank, x, y, z);

			break;
	}

	printf("%d: x=%d y=%d z=%d\n", my_rank, x, y, z);

	// finalize MPI	
    MPI_Finalize();
}