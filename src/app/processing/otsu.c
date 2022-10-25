#include "processing/otsu.h"
#include "pixels_op/pixels_op.h"
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

int otsu_threshold(float *hist, int nbpixels)
{
    float sumB = 0;
    float wB = 0;
    float max = 0.0;
    float sum1 = 0;
    int threshold = 0;
    for(int i = 0; i < 256; i++)
    {
        sum1 += i * hist[i];
    }
    for(int i = 0; i < 256; i++)
    {
        float wF = nbpixels - wB;
        if (wB > 0 && wF > 0)
        {
            float mF = (sum1 - sumB) / wF;
            int val = wB * wF * ((sumB/wB) - mF) * ((sumB / wB) - mF);
            if(val >= max)
            {
                threshold = i;
                max = val;
            }
        }
        wB = wB + hist[i];
        sumB += i * hist[i];
    }
    return threshold;
}

void otsu(SDL_Surface *image_surface)
{
    size_t width = image_surface->w;
    size_t height = image_surface->h;

    float hist[256] = {0};
    init_hist(image_surface, width, height, hist, width*height);

    Uint8 threshold = otsu_threshold(hist, width*height);
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
