#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

SDL_Surface* load_image(const char* file);

Uint32 grayscale(Uint32 pixel_color, SDL_PixelFormat* format);

void apply_grayscale(SDL_Surface* surface, SDL_PixelFormat* format);

void contrast(SDL_Surface* surface, SDL_PixelFormat* format);

void invert(SDL_Surface* surface, SDL_PixelFormat* format, int status);

void draw(SDL_Renderer* renderer, SDL_Texture* texture);

#endif
