#ifndef PIXELS_OP_H
#define PIXELS_OP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

Uint32 getpixel(SDL_Surface* surface, int x, int y);

void putpixel(SDL_Surface* s, int x, int y, Uint32 pixel);

#endif
