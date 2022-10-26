#include "image_process/color_treatement/otsu.h"
#include "image_process/toolbox/pixels_op.h"
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_pixels.h>

void init_hist(SDL_Surface* s, int width, int height, float* hist, int nbpixels)
{
    Uint8 r, g, b;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = getpixel(s, i, j);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);
            hist[r]++;
        }
    }
    for(int i = 0; i < 256; i++)
        hist[i] /= nbpixels;
}

Uint8 otsu_threshold(float* hist)
{
    //B stands for Background and F for foreground
    float sumB = 0, sum = 0;
    float total = 0;
    float wB = 0, wF = 0;
    float mB = 0, mF = 0;
    float varMax = 0;
    float threshold = 0;
    
    for(int i = 0; i < 256; i++)
    {
        sum += i * hist[i];
        total += hist[i];
    }
    
    for(int i = 0; i < 256; i++)
    {
        wB += hist[i];
        wF = total - wB;
        
        sumB += i * hist[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        double varmid = wB * wF * (mB - mF) * (mB- mF);

        if(varmid > varMax)
        {
            threshold = i;
            varMax = varmid;
        }
    }
    return (Uint8)threshold;
}

void otsu(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    float hist[256] = {0};
    init_hist(image_surface, width, height, hist, width*height);

    Uint8 threshold = otsu_threshold(hist);
    Uint8 r, g, b;

    for (size_t i = 0; i < width; i++)
    {
        for (size_t j = 0; j < height; j++)
        {
            Uint32 pixel = getpixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

            if (r > threshold)
                r = 255;
            else
                r = 0;

            pixel = SDL_MapRGB(image_surface->format, r, r, r);
            putpixel(image_surface, i, j, pixel);
        }
    }
}
