#include "rotation/rotation.h"
#include "pixels_op/pixels_op.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>

SDL_Surface* rotation(SDL_Surface* s, double degree)
{
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);
    double angle =(M_PI / 180) * degree;
    
    int w = s->w;
    int h = s->h;
    SDL_PixelFormat* pixel_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);

    int center_x = w / 2;
    int center_y = h / 2;

    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            int xp = (int)((x - center_x) * cos(angle) - (y - center_y) * sin(angle) + center_x);
            int yp = (int)((x - center_x) * sin(angle) - (y - center_y) * cos(angle) + center_y);
            if(0 <= xp && xp < w && 0 <= yp && yp < h)
                putpixel(output, x, y, getpixel(s, xp, yp));
            else
                putpixel(output, x, y, SDL_MapRGB(pixel_format, 0, 0, 0));
        }
    }
    return output;
}
