#include "decompose.h"

void decompose(SDL_Surface* surface, float* rValue)
{
    Uint8 r,g,b;
    Uint32* pixels = surface -> pixels;
    int w = surface -> w;
    int h = surface -> h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    for(int i=0; i<w*h;i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        float average = ((r+g+b)/3)/255;
        rValue[i] = ((int)(average+1))%2;
    }
}
