#include "tools.h"

SDL_Surface* load_image(const char* file)
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
    //Uint8 gray = 0.3*r + 0.59*g + 0.11*b;
    Uint8 gray = (r+b+g)/3;
    return SDL_MapRGB(format, gray, gray, gray);
}

void apply_grayscale(SDL_Surface* surface, SDL_PixelFormat* format)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_LockSurface(surface);
    for(int i = 0; i < len; i++)
        pixels[i] = grayscale(pixels[i], format);
    SDL_UnlockSurface(surface);
}
void invert(SDL_Surface* surface, SDL_PixelFormat* format, int status)
{
    if(status)
    {
        Uint32* pixels = surface->pixels;
        int len = surface->w * surface->h;
        SDL_LockSurface(surface);
        Uint8 r, g, b;
        for(int i = 0; i < len; i++)
        {

            SDL_GetRGB(pixels[i], format, &r, &g, &b);
            pixels[i] = SDL_MapRGB(format, 255 - r, 255 - g, 255 - b);
        }
        SDL_UnlockSurface(surface);
    }
    else
    {
        Uint32* pixels = surface->pixels;
        Uint32 pixel_ref = SDL_MapRGB(format, 100, 100, 100);
        int len = surface->w * surface->h;
        SDL_LockSurface(surface);
        for(int i = 0; i < len; i++)
        {
            if (pixels[i] < pixel_ref)
                pixels[i] = SDL_MapRGB(format, 255, 255, 255);
            else
                pixels[i] = SDL_MapRGB(format, 0, 0, 0);
        }
        SDL_UnlockSurface(surface);
    }

}

Uint8 contrast_fun(Uint8 c)
{
    if(c <= 255 / 2)
        return (Uint8)( (255/2) * SDL_pow((double) 2 * c / 255, 5));
    else
        return 255 - contrast_fun(255 - c);
}

void contrast(SDL_Surface* surface, SDL_PixelFormat* format)
{
    Uint32* pixels = surface->pixels;
    Uint8 r,g ,b;
    int len = surface->w * surface->h;
    SDL_LockSurface(surface);
    for(int i = 0; i < len; i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        r = contrast_fun(r);
        g = contrast_fun(g);
        b = contrast_fun(b);
        pixels[i] = SDL_MapRGB(format, r, g, b);
    }
    SDL_UnlockSurface(surface);
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
