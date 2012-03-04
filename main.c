#include "header.h"

int main (int agrc, char *argv[])
{
	int quit, ii, ij;
	SDL_Surface *screen;
	SDL_Event event;
	Uint32 *pix, r, g, b;
	snapshot s;

	quit = 0;

	find_dimensions_ASCII("final.dat");
	printf("Radius spanning [%f..%f]\n", rmin, rmax);

	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(500, 500, 32, SDL_SWSURFACE);
	pix = (Uint32*) screen->pixels;

	printf("Dimensions: (%d, %d)\n", dimt, dimr);
	load_snapshot_ASCII("final.dat", &s);

	printf("A range: [%f..%f]\n", s.minA, s.maxA);
	printf("B range: [%f..%f]\n", s.minB, s.maxB);

	for (ii=0; ii<500; ii++)
	{
		for (ij=0; ij<500; ij++)
		{
			*(pix+ii*500+ij) = color(&s, 
				(rmax/250.)*sqrt((ii-250)*(ii-250)+(ij-250)*(ij-250)),
				atan2(ij-250,ii-250));
		}
	}

	SDL_Flip(screen);

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) quit = 1;
		}
	}

	SDL_Quit();

	return EXIT_SUCCESS;
}

Uint32 color (snapshot *ss, float r, float theta)
{
	Uint8 red, green, blue;
	red = (Uint8) 255*(max(-interpolate(ss->B, r, theta),0))/(-ss->minB);
	green = (Uint8) 255*(max(interpolate(ss->B, r, theta),0))/(ss->maxB);
	blue = (Uint8) 155*(max(interpolate(ss->B, r, theta),0))/(ss->maxB) + 
				 155*(max(-interpolate(ss->B, r, theta),0))/(-ss->minB);

	if (red<0) red=0;
	if (red>255) red=255;
	if (green<0) green=0;
	if (green>255) green=255;
	if (blue<0) blue=0;
	if (blue>255) blue=255;
	
	return (red<<16)|(green<<8)|(blue);
}

float interpolate (float **data, float r, float theta)
{
	int ii, ij;
	if (r<rmin || theta<0.) return 0.0;
	if (r>rmax || theta>PI) return 0.0;

	ii = ((r-rmin)/(rmax-rmin))*(dimr-1.);
	ij = (theta/PI)*(dimt-1.);
/*	printf("int at %d %d\n", ii, ij);*/
	return data[ii][ij];
}
