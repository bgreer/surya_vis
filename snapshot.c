#include "header.h"

/* This file handles snapshot structs */
/* Both in reading files and freeing memory */

void load_snapshot_ASCII (char *fname, snapshot* ss)
{
	FILE *fp;
	int num, ii, ij;
	float theta, r, A, B;

	/* Allocate memory for A,B */
	ss->A = malloc(dimr*sizeof(float*));
	ss->B = malloc(dimr*sizeof(float*));
	for (ii=0; ii<dimr; ii++)
	{
		ss->A[ii] = malloc(dimt*sizeof(float));
		ss->B[ii] = malloc(dimt*sizeof(float));
	}

	ss->minB = ss->minA = FLT_MAX;
	ss->maxB = ss->maxA = -FLT_MAX;

	fp = fopen(fname, "r");
	ii = ij = 0;
	while (num=fscanf(fp, "\t%f\t%f\t%f%f", &theta, &r, &A, &B) == 4)
	{
		ss->A[ii][ij] = A;
		ss->B[ii][ij] = B;
		if (A > ss->maxA) ss->maxA = A;
		if (A < ss->minA) ss->minA = A;
		if (B > ss->maxB) ss->maxB = B;
		if (B < ss->minB) ss->minB = B;
		ij++;
		if (ij==dimt)
		{
			ij = 0;
			ii++;
		}
	}
	fclose(fp);
}

void find_dimensions_ASCII (char *fname)
{
	FILE *fp;
	int num, i;
	float theta, r, A, B, prevt, prevr;

	fp = fopen(fname, "r");
	prevt = prevr = -1.;
	while (num=fscanf(fp, "\t%f\t%f\t%f%f", &theta, &r, &A, &B) == 4)
	{
		if (prevr<0)
		{
			rmin = r;
			dimt = 1;
			dimr = 1;
		} else {
			if (r!=prevr)
			{
				dimt = 1;
				dimr++;
			} else {
				dimt++;
			}
		}	
		prevt = theta;
		prevr = r;
	}
	rmax = r;
	fclose(fp);
}
