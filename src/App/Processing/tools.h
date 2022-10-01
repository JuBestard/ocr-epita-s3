#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* Load_Image(const char* file);

Uint32 grayscale(Uint32 pixel_color, SDL_PixelFormat* format);

void apply_grayscale(SDL_Surface* surface);

void black_white_level(SDL_Surface* surface);

void draw(SDL_Renderer* renderer, SDL_Texture* texture);

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* blackwhiteTexture);

#endif
