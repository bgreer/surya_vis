#include "header.h"

int main (int agrc, char *argv[])
{
	int quit, ii, ij, w, h;
	SDL_Surface *screen;
	SDL_Event event;
	Uint32 *pix, r, g, b;
	snapshot s;

	quit = 0;
	/* Set window dimensions */
	w = 512;
	h = 512;
	printf("Dimensions: (%d, %d)\n", dimt, dimr);

	find_dimensions_ASCII("final.dat");
	printf("Radius spanning [%f..%f]\n", rmin, rmax);

	/* Initialize SDL things */
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE);
	pix = (Uint32*) screen->pixels;

	/* Load snapshot */
	load_snapshot_ASCII("final.dat", &s);
	printf("A range: [%f..%f]\n", s.minA, s.maxA);
	printf("B range: [%f..%f]\n", s.minB, s.maxB);

	/* Draw things to pixel buffer */
	for (ii=0; ii<h; ii++)
	{
		for (ij=0; ij<w/2; ij++)
		{
			*(pix+ii*w+ij) = color2(&s, 
				(rmax/(w/2))*sqrt((ii-h/2)*(ii-h/2)+(ij-w/2)*(ij-w/2)),
				atan2(-ij+w/2,ii-h/2));
		}
		for (ij=w/2; ij<w; ij++)
		{
			*(pix+ii*w+ij) = color(&s, 
				(rmax/(w/2))*sqrt((ii-h/2)*(ii-h/2)+(ij-w/2)*(ij-w/2)),
				atan2(ij-w/2,ii-h/2));
		}
	}

	/* Refresh screen */
	SDL_Flip(screen);
	
	/* Check for exit */
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

/* Color mapping for B field */
Uint32 color (snapshot *ss, float r, float theta)
{
	Uint8 red, green, blue;
	red = (Uint8) 255*(max(-interpolate(ss->B, r, theta),0))/(-ss->minB);
	blue = (Uint8) 255*(max(interpolate(ss->B, r, theta),0))/(ss->maxB);
	green = (Uint8) 127*(max(interpolate(ss->B, r, theta),0))/(ss->maxB) + 
				 127*(max(-interpolate(ss->B, r, theta),0))/(-ss->minB);
	if (red<0) red=0; if (red>255) red=255;
	if (green<0) green=0; if (green>255) green=255;
	if (blue<0) blue=0; if (blue>255) blue=255;
	return (red<<16)|(green<<8)|(blue);
}

/* Color mapping for A field */
Uint32 color2 (snapshot *ss, float r, float theta)
{
	Uint8 red, green, blue;
	red = (Uint8) 255*(max(-interpolate(ss->A, r, theta),0))/(-ss->minA);
	green = (Uint8) 255*(max(interpolate(ss->A, r, theta),0))/(ss->maxA);
	blue = (Uint8) 127*(max(interpolate(ss->A, r, theta),0))/(ss->maxA) + 
				 127*(max(-interpolate(ss->A, r, theta),0))/(-ss->minA);
	if (red<0) red=0; if (red>255) red=255;
	if (green<0) green=0; if (green>255) green=255;
	if (blue<0) blue=0; if (blue>255) blue=255;
	return (red<<16)|(green<<8)|(blue);
}

/* doesnt actually interpolate */
float interpolate (float **data, float r, float theta)
{
	int ii, ij;
	if (r<rmin || theta<0.) return 0.0;
	if (r>rmax || theta>PI) return 0.0;
	
	/* nearest-neighbor */
	ii = ((r-rmin)/(rmax-rmin))*(dimr-1.);
	ij = (theta/PI)*(dimt-1.);
	return data[ii][ij];
}
