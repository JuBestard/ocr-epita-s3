#include "edge_detection/sobel_operator.h"
#include "pixels_op/pixels_op.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

SDL_Surface* sobel_operator(SDL_Surface* surface)
{
    int width = surface->w;
    int height = surface->h;
    Uint8 intensity[width][height];

    SDL_PixelFormat* pformat = surface->format;
    int bpp = pformat->BitsPerPixel;

    Uint8 r, g, b;
    Uint32* pixels = surface->pixels;
    Uint32 pixel;

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            pixel = pixels[x + y * width];
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            intensity[x][y] = r;
        }
    }

    int kernelx[3][3] = 
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int kernely[3][3] =
    { 
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    SDL_Surface* output = SDL_CreateRGBSurfaceWithFormat(0,width, height, bpp, pformat->format);
    Uint32* pixelsOut = output->pixels;
    for(int y = 1; y < height-1; y++)
    {
        for(int x = 1; x < width-1; x++)
        {
            double magx = 0., magy = 0.;
            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    int xn = x + i - 1;
                    int yn = y + j - 1;
                    magx += intensity[xn][yn] * kernelx[i][j];
                    magy += intensity[xn][yn] * kernely[i][j];
                }
            }

            double color = sqrt(magx*magx + magy*magy);
            pixelsOut[x + y * width] = SDL_MapRGB(output->format, color, color, color);
        }
    }
    IMG_SavePNG(output, "out.png");
    return output;
}
