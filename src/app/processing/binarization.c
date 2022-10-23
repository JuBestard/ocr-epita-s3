#include "processing/binarization.h"
#include "processing/grayscale.h"
#include <SDL2/SDL_pixels.h>
#include <string.h>

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
    Uint32* pixels= s->pixels;
    for(int i = 0; i < s->w*s->h; i++)
    {
        Uint8 r, g, b;
        SDL_GetRGB(pixels[i], s->format, &r, &g, &b);
        if(r >= threshold)
            pixels[i] = SDL_MapRGB(s->format, 255, 255, 255);
        else
            pixels[i] = SDL_MapRGB(s->format, 0, 0, 0);
    }
}
