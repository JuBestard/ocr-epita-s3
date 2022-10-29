#include "image_process/toolbox/pixels_op.h"
#include "image_process/color_treatement/blur.h"
#include "math.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

void gauss_kernel(int size, double* kernel)
{
    //double sigma = (size - 1) / 6.;
    double sigma = 6.;
    double sum = 0.0;
    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            double val = exp(-((x*x + y*y)/2.0*pow(sigma,2.0)))/2*M_PI*pow(sigma,2.0);
            kernel[x + y * size] = val;
            sum += val;
        }
    }
    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            kernel[x + y*size] /= sum;
        }
    }
}

SDL_Surface* blur(SDL_Surface* s)
{
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    int w = s->w;
    int h = s->h;

    int size = 10;

    double* kernel = malloc(sizeof(double) * size * size);
    gauss_kernel(size, kernel);
    int sumr = 0, sumb = 0, sumg = 0;
    int offset = size/2;
    Uint32 pixel;
    Uint8 r, g, b;
    for(int x = offset; x < w - offset; x++)
    {
        for(int y = offset; y < h - offset; y++)
        {
            sumr = 0, sumg = 0, sumb = 0;
            for(int i = 0; i < size; i++)
            {
                for(int j = 0; j < size; j++)
                {
                    pixel = getpixel(s, x + i - offset, y + j - offset);
                    SDL_GetRGB(pixel,s->format, &r, &g, &b);
                    sumr += r * kernel[i + j * size];
                    sumg += g * kernel[i + j * size];
                    sumb += b * kernel[i + j * size];
                }
            }
            pixel = SDL_MapRGB(s->format, sumr, sumg, sumb);
            putpixel(output, x, y, pixel);
        }
    }
    free(kernel);
    return output;
}

