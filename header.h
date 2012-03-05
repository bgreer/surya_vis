#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <SDL/SDL.h>

#define PI 3.14159265
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))

/* Properties of all snapshots to be read in */
int dimr, dimt;
float rmin, rmax;

typedef struct
{
	double time;
	float **A, **B; /* A[r][theta] */
	float maxA, minA, maxB, minB;
} snapshot;


/* Prototypes */

/* main.c */
float interpolate (float **data, float r, float theta);
Uint32 color (snapshot *ss, float r, float theta);
Uint32 color2 (snapshot *ss, float r, float theta);

/* snapshot.c */
void load_snapshot_ASCII (char *fname, snapshot *ss);
void find_dimensions_ASCII (char *fname);
