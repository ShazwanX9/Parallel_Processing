/**
 * @file lab10.c
 * @author Shazwan Bin Sharum Mizan
 * @brief Shazwan's Answer for Parallel Processing Lab
 * @version 0.1
 * @date 2023-07-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/*
	In this assignment, just look for a `for-loop` 
	and decide to pragma omp it

	Keep in mind: parallel the printing 
	will mess up your output
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

float params[]={0, 1920,1080,-0.70174,0.156,1.2,500};

#define TEMPFILE "lab10.pgm"
#define MAXPIXEL 16
int ** picture;
void allocatePicture(int width, int height);
#define putpixel(x,y,value) picture[y][x]=value
void displayPicture(int width, int height);
 
typedef struct{
	double x,y;
}complex;
 
complex add(complex a,complex b){
	complex c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}
 
complex sqr(complex a){
	complex c;
	c.x = a.x*a.x - a.y*a.y;
	c.y = 2*a.x*a.y;
	return c;
}
 
double mod(complex a){
	return sqrt(a.x*a.x + a.y*a.y);
}
 
complex mapPoint(int width,int height,double radius,int x,int y){
	complex c;
	int l = (width<height)?width:height;
 
	c.x = 2*radius*(x - width/2.0)/l;
	c.y = 2*radius*(y - height/2.0)/l;
 
	return c;
}
 
void juliaSet(int width,int height,complex c,double radius,int n){
	int x,y,i;
	complex z0,z1;

	#pragma omp parallel for private(x,y,i,z0,z1)
	for(x=0;x<width;x++)
		for(y=0;y<height;y++){
			z0 = mapPoint(width,height,radius,x,y);
			for(i=1;i<=n;i++){
				z1 = add(sqr(z0),c);
				if(mod(z1)>radius){
					putpixel(x,y,i%MAXPIXEL);
					break;
				}
				z0 = z1;
			}
			if(i>n)
				putpixel(x,y,0);
		}
}
 
int main(int argC, char* argV[])
{
	int width, height, iterations;
	complex c;
	float radius;
 
	width = params[1];
	height = params[2];
	c.x = params[3];
	c.y = params[4];
	radius = params[5];
	iterations = params[6];
 
	clock_t begin=clock();
	allocatePicture(width,height);
	/* clock_t end=clock(); */
	/* clock_t begin=clock(); */
	juliaSet(width,height,c,radius,iterations);
	clock_t end=clock();
	
	printf("Time spent: %f seconds\n",(double)(end-begin)/CLOCKS_PER_SEC);
 
	displayPicture(width,height);
}

void allocatePicture(int width, int height)
{
	picture = (int **)malloc(sizeof(int*)*height);
	#pragma omp parallel for
	for (int i = 0; i<height; i++)
		picture[i]=(int*)malloc(sizeof(int)*width);
}

void displayPicture(int width, int height)
{	int x,y;
	FILE *outfile = fopen(TEMPFILE,"w");
	fprintf(outfile,"P2\n%d %d %d\n",width, height, MAXPIXEL);

	// cannot pragma omp parallel here because it will mess up your output lolz
	for (y=0; y<height; y++)
		for (x=0; x<width; x++)
			fprintf(outfile,"%d\n", picture[y][x]);
	fclose(outfile);
	system("display "TEMPFILE);
}