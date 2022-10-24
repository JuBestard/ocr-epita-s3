#include "pixels_op/pixels_op.h"
#include "processing/blur.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>

void blur(SDL_Surface* surface)
{
	float gaussian[3][3] =
	{
		{1./16., 2./16., 1./16.},
		{2./16., 4./16., 1./16.},
		{1./16., 2./16., 1./16.},
	};
	Uint32 pixel;
	float m = 0;
	float s = 0;
	for(int i = 0; i<surface->w; i++){
		for(int j = 0; j<surface->h; j++){
			s = 0;
			for(int a = -1; a<2; a++){
				for(int b = -1; b<2; b++){
					if(i+a < surface->w && i+a >= 0 && j+b <surface->h && j+b >= 0)
					{
						pixel = getpixel(surface, i+a, j+b);
						Uint8 r, g, b1;
						SDL_GetRGB(pixel, surface->format, &r, &g, &b1);
						m = gaussian[a+1][b+1];
						s += m*(float)r;
					}
				}
			}
			pixel = SDL_MapRGB(surface->format, (Uint8)s, (Uint8)s, (Uint8)s);
			putpixel(surface, i, j, pixel);
		}
	}
}
