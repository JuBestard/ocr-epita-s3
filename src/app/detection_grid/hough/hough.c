#include "detection_grid/hough/hough.h"
#include "image_process/toolbox/pixels_op.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

void hough(SDL_Surface* s)
{
    int width = s->w;
    int height = s->h;
    int rho = sqrt(width*width + height*height);
    int theta = 180;

    //initialise accumulator array
    int acc_array[rho][theta];

    for (size_t i = 0; i < rho; i++)
    {
        for (size_t j = 0; j < theta; j++)
            acc_array[i][j] = 0;
    }
    Uint32 pixel;
    Uint8 r, g, b;
    
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            pixel = get_pixel(s, x, y);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);

            //if white
            if (r+g+b == 765)
            {
                for (int t = 0; t < 180;t++)
                {
                    int p = x*cos(t) + y*sin(t);
                    acc_array[p][t]++;
                }
            }
        }
    }



}
