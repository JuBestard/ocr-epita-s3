#include "pixels_op/pixels_op.h"
#include "processing/blur.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>


void gaussian_kernel(int size, double* kernel)
{
    double sigma = (size - 1) / 6.;
    double sum = 0.0;
    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            double val = exp(-((x*x + y*y)/(2*pow(sigma,2.0))))/2*M_PI*pow(sigma,2.0);
            kernel[x + y * size] = val;
            sum += val;
        }
    }
    for(int x = 0; x < size; x++)
    {
        for(int y = 0; y < size; y++)
        {
            kernel[x + y * size] /= sum;
        }
    }
}
SDL_Surface* blur(SDL_Surface* s)
{
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    int w = s->w;
    int size = w/100;
    double* kernel = malloc(sizeof(double)*size*size);
    gaussian_kernel(size, kernel);
    /*double kernel[size][size];
    double sigma = (size - 1) / 6.;
    double sum = 0.0;// For accumulating the kernel values
    for (int x = 0; x < size; x++) 
    {
        for (int y = 0; y < size; y++) 
        {
            kernel[x][y] = exp(-((x*x + y*y)/(2 * sigma*sigma)))/2*M_PI*sigma*sigma;
            // Accumulate the kernel values
            sum += kernel[x][y];
        }
    };

    // Normalize the kernel
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            kernel[x][y] /= sum;
        }
    };*/
    
    int h = s-> h;
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
                    sumr += r * kernel[i + j*size];
                    sumg += g * kernel[i + j*size];
                    sumb += b * kernel[i + j*size];
                }
            }
            pixel = SDL_MapRGB(s->format, sumr, sumg, sumb);
            putpixel(output, x, y, pixel);
        }
    }


	IMG_SavePNG(output, "blur.png");
    free(kernel);
    return output;
}

