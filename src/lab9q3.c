/**
 * @file lab9q3.c
 * @author Shazwan Bin Sharum Mizan
 * @brief Shazwan's Answer for Parallel Processing Lab
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <omp.h>
#define ARRAYSIZE ('Z'-'A'+1)

int main(int argc, char *argv[])
{
    FILE *infile;
    int i, count[ARRAYSIZE]={0};
    int fail_count[ARRAYSIZE]={0};
    long int size;
    char *buffer;
    double start, end;

    // infile = fopen(argv[1],"r");
	printf("just another dummy data -> %s", "lab9q3test.txt");
    infile = fopen("lab9q3test.txt","r");

    if (infile)
    {
        fseek(infile,0L, SEEK_END); 
        size = ftell(infile); 
        fseek(infile,0L, SEEK_SET); 

        buffer = malloc(size); 
        fread(buffer,1,size,infile); 

        start = omp_get_wtime();

		// notice that I only reduce the count but not fail count
		/*
			Reduction:
			each thread gets its own copy of `count`, 
			and at the end of the parallel for region, 
			all the local copies of `count` are 
			`+` [summed] up to get the grand total
		*/
        #pragma omp parallel for reduction(+:count)
        for (i=0; i<size; i++)
            if (isalpha(buffer[i])) {
                count[tolower(buffer[i])-'a']++;
				fail_count[tolower(buffer[i])-'a']++;
			}

        end = omp_get_wtime();

        printf("CPU time used: %f seconds\n",end - start);

        for (i=0; i<ARRAYSIZE; i++) {
            printf("\t%c -> %d vs %d\n", i+'A', count[i], fail_count[i]);
		}
    }

    return 0;
}