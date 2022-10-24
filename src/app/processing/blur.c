#include "pixels_op/pixels_op.h"
#include "processing/blur.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

SDL_Surface* blur(SDL_Surface* s)
{
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    /*double sigma = 1;
    int W = 5;
    double kernel[W][W];
    double mean = W/2;
    double sum = 0.0;// For accumulating the kernel values
    for (int x = 0; x < W; ++x) 
    {
        for (int y = 0; y < W; ++y) 
        {
            kernel[x][y] = exp(-0.5*(pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)))/(2*M_PI*sigma*sigma);

        // Accumulate the kernel values
            sum += kernel[x][y];
        }
    }

    // Normalize the kernel
    for (int x = 0; x < W; ++x)
    {
        for (int y = 0; y < W; ++y)
        {
            kernel[x][y] /= sum;
        }
    }*/
    
    float kernel[5][5] = 
    {  
        {0.0030,    0.0133,    0.0219,    0.0133,    0.0030},
        {0.0133,    0.0596,    0.0983,    0.0596,    0.0133},
        {0.0219,    0.0983,    0.1621,    0.0983,    0.0219},
        {0.0133,    0.0596,    0.0983,    0.0596,    0.0133},
        {0.0030,    0.0133,    0.0219,    0.0133,    0.0030},
    };

	float gaussian5[5][5] =
	{
        {1./256., 4./256., 6./256., 4./256., 1./256.},
        {4./256., 16./256., 24./256., 16./256., 4./256.},
        {6./256., 24./256., 36./256., 24./256., 16./256.},
        {4./256., 16./256., 24./256., 16./256., 4./256.},
        {1./256., 4./256., 6./256., 4./256., 1./256.},
	};

    float sumGauss = 0;
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 5; j++)
        {
            sumGauss += gaussian5[i][j];
        }
    }

	float gaussian3[3][3] =
	{
		{1./16., 2./16., 1./16.},
		{2./16., 4./16., 1./16.},
		{1./16., 2./16., 1./16.},
	};

    int w = s->w;
    int h = s-> h;
    int sumr = 0, sumb = 0, sumg = 0;
    int offset = 2;
    Uint32 pixel;
    Uint8 r, g, b;
    for(int x = offset; x < w - offset; x++)
    {
        for(int y = offset; y < h - offset; y++)
        {
            sumr = 0, sumg = 0, sumb = 0;
            for(int i = 0; i < 5; i++)
            {
                for(int j = 0; j < 5; j++)
                {
                    pixel = getpixel(s, x + i - offset, y + j - offset);
                    SDL_GetRGB(pixel,s->format, &r, &g, &b);
                    sumr += r * kernel[i][j];
                    sumg += g * kernel[i][j];
                    sumb += b * kernel[i][j];
                }
            }
            pixel = SDL_MapRGB(s->format, sumr/sumGauss, sumg/sumGauss, sumb/sumGauss);
            putpixel(output, x, y, pixel);
        }
    }


	/*Uint32 pixel;
    int s = 0;
    int m = 0;
	for(int i = 0; i<surface->w; i++)
    {
		for(int j = 0; j<surface->h; j++)
        {
			s = 0;
			for(int a = -1; a<4; a++)
            {
				for(int b = -1; b<4; b++)
                {
					if(i+a < surface->w && i+a >= 0 && j+b <surface->h && j+b >= 0)
					{
						pixel = getpixel(surface, i+a, j+b);
						Uint8 r, g, b1;
						SDL_GetRGB(pixel, surface->format, &r, &g, &b1);
						m = gaussian5[a+1][b+1];
						s += m*(float)r;
					}
				}
			}
			pixel = SDL_MapRGB(surface->format, (Uint8)s, (Uint8)s, (Uint8)s);
			putpixel(surface, i, j, pixel);
		}
	}*/
    IMG_SavePNG(output, "blur.png");
    return output;
}

