/**
 * @file lab6q1.c
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
        printf("USE THE DUMMY DATA LOL\n");
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

    // paralellism logic
	
	/*
		3a. Split up the calculation task. 
		I suggest that you split it on the y axis 
		and have each processor compute a whole array in the x axis.
	*/
    for (y=myrank; y<ysize; y+=numprocs)
    { 
		/* 
			3b. Each process should only allocate the memory 
			for the rows they are calculating, so as not to 
			waste memory (except process 0 will need to 
			allocate memory for the rows it receives as well.)
		*/
        picture[y] = (int*)malloc(sizeof(int)*xsize);
        for (x=0; x<xsize; x++)
            picture[y][x] = julia(x,y);

		/*
			4. Send all the x arrays from the other 
			processes back to process 0. 
			I suggest you use y as the tag so that 
			you know which row you are receiving.
		*/
        if (myrank)
            MPI_Send(picture[y], xsize, MPI_INT, 0, y, MPI_COMM_WORLD);
    }

    // output file
    if (!myrank) {
        for (y=0; y<ysize; y++) {
            if (y%numprocs==0) continue;
            picture[y] = (int*)malloc(sizeof(int)*xsize);
            MPI_Recv(picture[y], xsize, MPI_INT, MPI_ANY_SOURCE, y, MPI_COMM_WORLD, MPI_SUCCESS);
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

	MPI_Finalize();
}
