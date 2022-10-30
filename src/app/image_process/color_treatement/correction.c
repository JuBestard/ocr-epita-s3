#include "image_process/toolbox/pixels_op.h"
#include "tools.h"
#include "image_process/color_treatement/correction.h"
#include "math.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

 #define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

SDL_Surface* c_gamma(SDL_Surface* s)
{
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    int w = s->w;
    int h = s->h;

    Uint32 pixel;
    Uint8 r, g, b;
    float c_gamma = 2.0f / (float) (255 - average_pixel(s));
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            pixel = getpixel(s, x, y);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);
            int nr = Truncate(255.0 * pow((r / 255.0),c_gamma));
            int ng = Truncate(255.0 * pow((g / 255.0),c_gamma));
            int nb = Truncate(255.0 * pow((b / 255.0),c_gamma));
            pixel = SDL_MapRGB(output->format, nr, ng, nb);
            putpixel(output, x, y, pixel);
        }
    }
    return output;
}

SDL_Surface* c_contrast(SDL_Surface* s)
{
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    int w = s->w;
    int h = s->h;

    Uint32 pixel;
    Uint8 r, g, b;
    int av = average_pixel(s);

    int factor = (259 * (av + 255)) / (255 * (259 - av));

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            pixel = getpixel(s, x, y);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);
            r = Truncate(factor * (r - av) + av);
            g = Truncate(factor * (g - av) + av);
            b = Truncate(factor * (b - av) + av);
            int mini = min(r,min(g,b));
            pixel = SDL_MapRGB(output->format, mini, mini, mini);
            putpixel(output, x, y, pixel);
        }
    }
    return output;
}
