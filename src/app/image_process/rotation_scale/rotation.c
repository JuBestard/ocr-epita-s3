#include "image_process/rotation_scale/rotation.h"
#include "image_process/toolbox/pixels_op.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
SDL_Surface* rotation(SDL_Surface* s, double degree)
{

    int w = s->w;
    int h = s->h;

    SDL_PixelFormat* pixel_format = s->format;
    int bpp = s->format->BitsPerPixel;

    SDL_Surface* output = SDL_CreateRGBSurfaceWithFormat(0,w,h,bpp, pixel_format->format);

    double center_x = w / (double) 2;
    double center_y = h / (double) 2;
    
    double angle = degree * (M_PI / 180.0);
    double sinus = sin(angle);
    double cosinus = cos(angle);

    Uint32 black = SDL_MapRGB(pixel_format, 0, 0, 0);   

    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            double xp = (x - center_x) * cosinus - (y - center_y) * sinus + center_x;
            double yp = (x - center_x) * sinus + (y - center_y) * cosinus + center_y;

            if(0 <= xp && xp < w && 0 <= yp && yp < h)
            {
                putpixel(output, x, y, getpixel(s, (int)xp, (int)yp));
            }
            else
                putpixel(output, x, y, black);
        }
    }
    return output;
}
