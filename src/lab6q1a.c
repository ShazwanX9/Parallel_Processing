/**
 * @file lab6q1a.c
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
#include <string.h>
#include <mpi.h>

#define BUFSIZE 256

// global variable given in question
int xsize, ysize, maxIteration;
double cx, cy, xscale, xoffset, yscale, yoffset;

// Function given in the question
int julia(int x, int y)
{ double zx = (double)x/xsize *xscale -xoffset;
  double zy = (double)y/ysize *yscale -yoffset;
  
  int iteration = 0;
  
  while((iteration<maxIteration) && (zx * zx + zy * zy < 4))
  {
	  double xtemp = zx * zx - zy * zy;
	  zy = 2 * zx * zy + cy;
	  zx = xtemp + cx;
	  
	  iteration++;
  }
  
  return iteration;
}

int main(int argc, char*argv[])
{ 
    int x, y;
    int **picture;
    char buff[BUFSIZE];
    FILE *outfile;

    // MPI variable
	int numprocs, myrank;

	// initialize MPI with parameter
	MPI_Init(&argc, &argv);

	// get number of processor
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// get current processor id
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	/* 
		1. Isolate the parts that talk to the user (both input and output) 
		and restrict those to be done only by process 0.
	*/
    if (!myrank) {
        printf("Program to generate a Julia fractal\n");
        printf("What file should I store the picture in? [E.g. julia.pgm] ");fflush(stdout);
        fgets(buff,BUFSIZE,stdin); buff[strlen(buff)-1]=0; // Remove the ending line feed
        // printf("How wide and tall should the picture be? [E.g. 1920 1080] ");fflush(stdout);
        // scanf("%d %d", &xsize, &ysize);
        // printf("What is the x scale and x offset? [E.g. 3.5 1.7] ");fflush(stdout);
        // scanf("%lf %lf", &xscale, &xoffset);  
        // printf("What is the y scale and y offset? [E.g. 2.0 1.0] ");fflush(stdout);
        // scanf("%lf %lf", &yscale, &yoffset);   
        // printf("What are the x and y components of the Julia constant? [E.g -0.79 0.15] ");fflush(stdout);
        // scanf("%lf %lf", &cx, &cy);
        // printf("How many times to iterate? [E.g. 512] "); fflush(stdout);
        // scanf("%d", &maxIteration);

        // Just a dummy data here lol
        printf("USE THE DUMMY DATA LOLZ\n");
        xsize = 1920;
        ysize = 1080;
        xscale = 3.5;
        xoffset = 1.7;  
        yscale = 2.0;
        yoffset = 1.0;   
        cx = -0.79;
        cy = 0.15;
        maxIteration = 512;

        // xsize = 1920;
        // ysize = 1080;
        // xscale = 3.5;
        // xoffset = 1.7;  
        // yscale = 2.5;
        // yoffset = 1.25;   
        // cx = 0.285;
        // cy = 0.01;
        // maxIteration = 512;
    }

	/* 
		2. Broadcast the information received from the user 
		that other processes need to know to compute 
		their part to all the other processes.
	*/
	MPI_Bcast( &xsize, 1, MPI_INT, 0, MPI_COMM_WORLD );
	MPI_Bcast( &ysize, 1, MPI_INT, 0, MPI_COMM_WORLD );
	MPI_Bcast( &xscale, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( &yscale, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( &xoffset, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( &yoffset, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( &cx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( &cy, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	MPI_Bcast( &maxIteration, 1, MPI_INT, 0, MPI_COMM_WORLD );

    // initialize picture table
    picture = (int**)malloc(sizeof(int*)*ysize);

	/*
		P/S: I use different variable just so you could differ it usage
		MPI_Gather only works on a flat array
	*/
	int *local_picture;
	int *gather_picture;

    // paralellism logic
	int ysplit = ysize/numprocs;

	// initialize flat array
	local_picture = (int*)malloc(sizeof(int*)*ysplit*xsize);
	gather_picture = (int*)malloc(sizeof(int*)*ysize*xsize);

	// the computation logics
    for (y=0; y<ysplit; y++)
        for (x=0; x<xsize; x++)
            local_picture[xsize*y+x] = julia(x,y+(ysplit*myrank));

	// Gather all the local_picture -> gather_picture
	MPI_Gather(
		local_picture, ysplit*xsize, MPI_INT, 
		gather_picture, ysplit*xsize, MPI_INT, 
		0, MPI_COMM_WORLD
	);

    // output file
    if (!myrank) {
		printf(
			"%d/%d=%d; so the value after %d need to be compute separately.\n", 
			ysize, numprocs, ysplit, ysplit*numprocs
		);
 
		for (y=0; y<ysize; y++) {
			// initialize initiale 2D array
            picture[y] = (int*)malloc(sizeof(int)*xsize);
			for (x=0; x<xsize; x++)
				// assigning the gathered data back to original array
				if (y < ysplit*numprocs)
					picture[y][x] = gather_picture[xsize*y+x];
				// dont forget the remainder
 				else
					picture[y][x] = julia(x, y);
		}

		/*
			5. Process 0 should print the results to the output file.
		*/
        outfile=fopen(buff,"w");
        fprintf(outfile,"P2\n%d %d %d\n", xsize, ysize, maxIteration);
        for(y=0; y<ysize; y++)
            for (x=0; x<xsize; x++)
				fprintf(outfile,"%d\n", picture[y][x]);

		fclose(outfile);
	}

	// finalize MPI	
	MPI_Finalize();
}
