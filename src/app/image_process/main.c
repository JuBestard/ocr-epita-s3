#include "rotation_scale/rotation.h"
#include "rotation_scale/scale.h"
#include "color_treatement/otsu.h"
#include "color_treatement/grayscale.h"
#include "color_treatement/blur.h"
#include "../tools.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <string.h>

#include "err.h"
#include "math.h"
void event_loop(SDL_Renderer* renderer, SDL_Texture* texture)
{
   SDL_Event event;
   while(1)
   {
       SDL_WaitEvent(&event);
       switch(event.type)
       {
            case SDL_QUIT:
               return;
            case SDL_WINDOWEVENT:
               if(event.window.event == SDL_WINDOWEVENT_RESIZED)
               {
                   draw(renderer, texture);
               }
               break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    return;
                break;
       }
   }
}

int usage()
{
    printf("%s\n", "Name : OCR");
    printf("%s\n", "Description : Sudoku Solver");
    printf("%s\n", "Usage : [mode] [image] [options]");
    printf("%s\n", "        [mode] : color -> color processing");
    printf("%s\n", "                 rotate -> manual rotation of an image (the degree of the rotation is given in option)");
    printf("%s\n", "        [options] : ONLY FOR THE ROTATION : the degree of the rotation");
    return EXIT_FAILURE;
}

int rotate(char* path, double degree)
{
    SDL_Surface* surface = load_image(path);

    SDL_Surface* surface_rotated = rotation(surface, degree);

    SDL_SaveBMP(surface_rotated, "out.bmp");
    SDL_FreeSurface(surface_rotated);
    return EXIT_SUCCESS;
}

int color_treatement(char* path)
{
    SDL_Surface* surface = load_image(path);

    grayscale(surface);
    SDL_Surface* surface_blur = blur(surface);
    otsu(surface_blur);

    SDL_SaveBMP(surface_blur, "out.bmp");
    
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface_blur);

    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    // Checks the number of arguments
    char* color = "color";
    char* srotate = "rotate";
    if(argc <= 2)
        return usage();
    if(argc == 3 && strcmp(argv[1], color) != 0)
        return usage();
    if (argc == 3)
        return color_treatement(argv[2]);
    if(argc == 4 && strcmp(argv[1], srotate) != 0)
        return usage();
    if(argc == 4)
        return rotate(argv[2], atof(argv[3]));
    else
        return usage();
}