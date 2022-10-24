#include "pixels_op/pixels_op.h"
#include "processing/binarization.h"
#include "processing/grayscale.h"
#include "processing/blur.h"

#include <string.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>

double sum_pixels(SDL_Surface* s)
{
    double sum = 0;
    Uint8 r, g, b;
    Uint32 pixel;
    for(int i = 0; i < s->w; i++)
    {
        for(int j = 0; j < s->h; j++)
        {
            pixel = getpixel(s, i, j);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);
            sum += (double)r;
        }
    }

    return sum;
}

void binarize(SDL_Surface* s)
{
    double threshold = sum_pixels(s)/(s->w * s->h);

    Uint8 r, g, b;
    Uint32 pixel;
    int w = s->w;
    int h = s->h;
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            pixel = getpixel(s, i, j);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);
            if(r >= threshold)
                putpixel(s, i, j, SDL_MapRGB(s->format, 255, 255, 255));
            else
                putpixel(s, i, j, SDL_MapRGB(s->format, 0, 0, 0));
        }
    }
}
