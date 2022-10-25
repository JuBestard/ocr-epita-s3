#include "rotation/rotation.h"
#include "pixels_op/pixels_op.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
SDL_Surface* rotation(SDL_Surface* s, double degree)
{
    double angle = (M_PI / 180.) * degree;
    int w = s->w;
    int h = s->h;
    SDL_PixelFormat* pixel_format = s->format;
    SDL_Surface* output = SDL_CreateRGBSurfaceWithFormat(0,w,h,pixel_format->BitsPerPixel, pixel_format->format);

    double center_x = w / 2.;
    double center_y = h / 2.;
    

    Uint32* pixelsOut = output->pixels;
    Uint32* pixelsIn = s->pixels;

    Uint32 pixelIn;
    Uint32 pixelOut;
    Uint8 r, g, b;
    
    Uint32 black = SDL_MapRGB(pixel_format, 0, 0, 0);   

    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            double xp = (x - center_x) * cos(angle) - (y - center_y) * sin(angle) + center_x;
            double yp = (x - center_x) * sin(angle) - (y - center_y) * cos(angle) + center_y;

            if(0 <= xp && xp < w && 0 <= yp && yp < h)
            {
                pixelIn = pixelsIn[(int)xp + (int)yp * w];
                SDL_GetRGB(pixelIn, pixel_format, &r, &g, &b);
                pixelOut = SDL_MapRGB(pixel_format, r, g, b);
                pixelsOut[x + y * w] = pixelOut;
                /*pixel = getpixel(s, (int)xp, (int)yp);
                putpixel(output, x, y, pixel);*/
            }
            else
            {
                //putpixel(output, x, y, black);
                pixelsOut[x + y * w] = black;
            }
        }
    }
    IMG_SavePNG(output, "out.png");
    return output;
}
