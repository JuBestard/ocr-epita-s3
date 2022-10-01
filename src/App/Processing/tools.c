#include "tools.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

SDL_Surface* Load_Image(const char* file)
{
    SDL_Surface* tmp = IMG_Load(file);
    SDL_Surface* off = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return off;
}

Uint32 grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    Uint8 gray = 0.3*r + 0.59*g + 0.11*b;
    return SDL_MapRGB(format, gray, gray, gray);
}

void apply_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format =surface-> format;
    SDL_LockSurface(surface);
    for(int i = 0; i < len; i++)
        pixels[i] = grayscale(pixels[i], format);
    SDL_UnlockSurface(surface);
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void black_white_level(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    Uint32 pixelRef = SDL_MapRGB(format, 128, 128, 128);
    SDL_LockSurface(surface);
    for(int i = 0; i < len; i++)
    {
        if(pixels[i] > pixelRef)
            pixels[i] = SDL_MapRGB(format, 255, 255, 255);
        else
            pixels[i] = SDL_MapRGB(format, 0, 0, 0);
    }
    SDL_UnlockSurface(surface);
}

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* blackwhite)
{
   SDL_Event event;
   SDL_Texture* t = texture;
   while(1)
   {
       SDL_WaitEvent(&event);
       switch(event.type)
       {
            case SDL_QUIT:
               return;
            case SDL_WINDOWEVENT:
               if(event.window.event == SDL_WINDOWEVENT_RESIZED)
               {
                   draw(renderer, texture);
               }
               break;
            case SDL_KEYDOWN:
                t = t == texture ? blackwhite : texture;
                draw(renderer, t);
                break;
       }
   }
}
