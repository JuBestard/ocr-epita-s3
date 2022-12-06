#include "detection_grid/segmentation/segmentation.h"
#include "image_process/rotation_scale/scale.h"
#include "image_process/toolbox/pixels_op.h"
#include "SDL2/SDL_surface.h"
#include "tools.h"
#include <SDL2/SDL_surface.h>
#include <sys/stat.h>
void clean(SDL_Surface* image)
{
    //__clean(image);
    int limitw = image->w/7;
    int limith = image->h/7;
    Uint32 white = SDL_MapRGB(image->format, 255, 255, 255);

    // up clean

    for (int i = 0; i < (limith * 7)/5; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            putpixel(image, j, i, white);
        }
    }
    
    // down clean

    for (int i = image->h - limith; i < image->h; i++)
    {
        for (int j = 0; j < image->w; j++)
        {
            putpixel(image, j, i, white);
        }
    }

    // left clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = 0; j < (limitw * 7)/5; j++)
        {
            putpixel(image, j, i, white);
        }
    }
    
    // right clean

    for (int i = 0; i < image->h; i++)
    {
        for (int j = image->w - limitw; j < image->w; j++)
        {
            putpixel(image, j, i, white);   
        }
    }
}

void splitting(char* path)
{
    SDL_Surface* s = load_image(path);
    int w = s->w;
    int h = s->h;
    mkdir("out/split/", S_IRWXU);

    int ratio = w/9;
    int n = 0;
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            SDL_Surface* dst = SDL_CreateRGBSurface(0, w/9, h/9, 32, 0, 0, 0, 0);
            SDL_Rect rect = {i * w/9, j * h/9, w/9, h/9};
            SDL_BlitSurface(s, &rect, dst, NULL);
            clean(dst);
            SDL_Surface* final = resize(dst, 28, 28);
            char file[200];
            snprintf(file, 200, "out/split/%i.bmp", n++);
            SDL_SaveBMP(final, file);
            SDL_FreeSurface(dst);
            SDL_FreeSurface(final);
        }
    }
    SDL_FreeSurface(s);
}
