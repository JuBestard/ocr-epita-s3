#include "detection_grid/segmentation/segmentation.h"
#include "image_process/toolbox/pixels_op.h"
#include "SDL2/SDL_surface.h"
#include "tools.h"
#include <SDL2/SDL_surface.h>
#include <sys/stat.h>

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
            //clean(dst);
            char file[200];
            snprintf(file, 200, "out/split/%i.bmp", n++);
            SDL_SaveBMP(dst, file);
            SDL_FreeSurface(dst);
        }
    }
    SDL_FreeSurface(s);
}
