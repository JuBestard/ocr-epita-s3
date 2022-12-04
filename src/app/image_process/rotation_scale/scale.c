#include "image_process/rotation_scale/scale.h"
#include "image_process/toolbox/pixels_op.h"
#include <SDL2/SDL_surface.h>
#include "math.h"

SDL_Surface* resize(SDL_Surface* s, int newW, int newH)
{
    int w = s->w;
    int h = s->h;

    SDL_PixelFormat* pformat = s->format;
    int bpp = s->format->BitsPerPixel;


    SDL_Surface* output = SDL_CreateRGBSurfaceWithFormat(0,newW,newH,
            bpp, pformat->format);

    int xratio = w/newW;
    int yratio = h/newH;

    for(int x = 0; x < output->w; x++)
    {
        for(int y = 0; y < output->h; y++)
        {
            int xp = floor(x * xratio);
            int yp = floor(y * yratio);
            putpixel(output,x,y,getpixel(s,xp,yp));
        }
    }

    return output;
}
