#include "pixels_op/pixels_op.h"
#include "processing/binarization.h"
#include "processing/grayscale.h"
#include "processing/blur.h"

#include <string.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>

double sum_pixels(SDL_Surface* s)
{
    Uint32* pixels = s->pixels;
    double sum = 0;
    for(int i = 0; i < s->w * s->h; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], s->format, &r, &g, &b);
        Uint8 gray = 0.3*r + 0.59*g + 0.11*b;        
        sum+= (double)gray;
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
    blur(s);
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

    /*for(int i = 0; i < s->w*s->h; i++)
    {
        SDL_GetRGB(pixels[i], output->format, &r, &g, &b);
        if(r >= threshold)
            pixels[i] = SDL_MapRGB(output->format, 255, 255, 255);
        else
            pixels[i] = SDL_MapRGB(output->format, 0, 0, 0);
    }*/
}
