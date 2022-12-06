#include "tools.h"
#include "image_process/toolbox/pixels_op.h"
SDL_Surface* load_image(const char* file)
{
    SDL_Surface* tmp = IMG_Load(file);
    if(tmp == NULL)
    {
        printf("Error : %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Surface* off = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return off;
}

int Truncate(int value)
{
    if(value < 0)
        return 0;
    if(value > 255)
        return 255;
    return value;
}

int average_pixel(SDL_Surface* s)
{
    Uint32 pixel;
    Uint8 r, g, b;
    int sr = 0, sg = 0, sb = 0;
    for(int y = 0; y < s->h; y++)
    {
        for(int x = 0; x < s->w; x++)
        {
            pixel = getpixel(s, x, y);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);
            sr += r;
            sg += g;
            sb += b;
        }
    }
    int nbpixels = s->w * s->h;
    return ((sr / nbpixels) + (sg / nbpixels) + (sb / nbpixels)) / 3;
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
