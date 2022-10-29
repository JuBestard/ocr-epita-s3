#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

SDL_Surface* load_image(const char* file);

void draw(SDL_Renderer* renderer, SDL_Texture* texture);
int average_pixel(SDL_Surface* s);
int Truncate(int value);
#endif
