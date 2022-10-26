#include "color_treatement/grayscale.h"

Uint32 _grayscale(Uint32 pc, SDL_PixelFormat* f)
{
    Uint8 r, g, b;
    SDL_GetRGB(pc, f, &r, &g ,&b);
    Uint8 gray = 0.3*r + 0.59*g + 0.11*b;
    return SDL_MapRGB(f, gray, gray, gray);
}

void grayscale(SDL_Surface* s, SDL_PixelFormat* f)
{
    Uint32* pixels = s->pixels;
    int len = s->w*s->h;
    SDL_LockSurface(s);
    for(int i = 0; i < len; i++)
        pixels[i] = _grayscale(pixels[i], f);
    SDL_UnlockSurface(s);
}
