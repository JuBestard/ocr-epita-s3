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

    int x_scale = w/output->w;
    int y_scale = h/output->h;

    for(int x = 0; x < output->w; x++)
    {
        for(int y = 0; y < output->h; y++)
        {
            int xp = floor(x * x_scale);
            int yp = floor(y * y_scale);
            putpixel(output,x,y,getpixel(s,xp,yp));
        }
    }

    return output;
}
