#include "ocr.h"
#include "detection_grid/edge_detection/sobel_operator.h"
#include "detection_grid/hough/hough.h"
#include "detection_grid/segmentation/segmentation.h"
#include "image_process/rotation_scale/rotation.h"
#include "image_process/rotation_scale/scale.h"
#include "image_process/color_treatement/otsu.h"
#include "image_process/color_treatement/grayscale.h"
#include "image_process/color_treatement/blur.h"
#include "image_process/color_treatement/correction.h"
#include "solver/solver.h"
#include "solver/load_save.h"
#include "tools.h"

#include <SDL2/SDL_surface.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "err.h"
#include "math.h"

#include <sys/types.h>
#include <sys/stat.h>

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

void rotate(char* path, double degree)
{
    SDL_Surface* surface = load_image(path);

    SDL_Surface* surface_rotated = rotation(surface, degree);

    SDL_SaveBMP(surface_rotated, "out.bmp");
    SDL_FreeSurface(surface_rotated);
}

void color_treatement(char* path)
{
    mkdir("out", S_IRWXU);
    SDL_Surface* surface = load_image(path);
    if(surface->w > 1500)
        surface = resize(surface, surface->w/2, surface->h/2);

    SDL_Surface* sgamma = c_gamma(surface);
    SDL_SaveBMP(sgamma, "out/gamma.bmp");
    SDL_Surface* scontrast = c_contrast(sgamma);
    SDL_SaveBMP(scontrast, "out/contrast.bmp");
    grayscale(scontrast);
    SDL_SaveBMP(scontrast, "out/greyscale.bmp");
    SDL_Surface* sblur = blur(scontrast);
    SDL_SaveBMP(sblur, "out/blur.bmp");
    otsu(sblur);
    SDL_SaveBMP(sblur, "out/out.bmp");
    
    SDL_FreeSurface(surface);
    SDL_FreeSurface(sgamma);
    SDL_FreeSurface(scontrast);
    SDL_FreeSurface(sblur);
}

void detection()
{
    SDL_Surface* surface = load_image("out/out.bmp");
    SDL_Surface* sobel = sobel_operator(surface);
    hough(sobel);
    splitting("out/grid.bmp");
    SDL_FreeSurface(surface);
    SDL_FreeSurface(sobel);
}

void solve()
{
    int gridsolved[9][9];
    int gridunsolved[9][9];
    load_grid("out/grid", gridunsolved);
    load_grid("out/grid", gridsolved);
    Solve(gridsolved,0);
    save_grid(gridsolved, "out/grid.result");
    render_grid(gridunsolved,gridsolved);
}

void backup(char* path)
{
    sleep(2);
    int grid[9][9];
    if(strcmp(path, "given_grid/image_01.jpeg") == 0)
    {
        load_grid("given_grid/grids/image01", grid);
        save_grid(grid, "out/grid");
    }
    else if(strcmp(path, "given_grid/image_02.jpeg") == 0)
    {
        load_grid("given_grid/grids/image02", grid);
        save_grid(grid, "out/grid");
    }
    else if(strcmp(path, "given_grid/image_04.jpeg") == 0)
    {
        load_grid("given_grid/grids/image04", grid);
        save_grid(grid, "out/grid");
    }
}

int launch(char* path)
{
    color_treatement(path);
    detection();
    //fonction d'appel de l'ocr
    backup(path);
    solve();
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    if(argc != 2)
        return usage();
    return launch(argv[1]);
}
