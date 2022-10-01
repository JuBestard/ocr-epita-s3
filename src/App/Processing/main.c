#include "tools.h"
#include <SDL2/SDL_render.h>
#include "err.h"

int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE,"%s", SDL_GetError());
    // - Create a window.
    SDL_Window* window = SDL_CreateWindow("Display Image", 0, 0, 640, 400,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    // - Create a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // - Create a surface from the colored image.
    SDL_Surface* surface = Load_Image(argv[1]);

    // Gets the width and the height of the texture.
    int w = surface->w;
    int h = surface->h;

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, w, h);
    // - Convert the surface into grayscale.
    apply_grayscale(surface);
    // - Create a new texture from the grayscale surface.
    SDL_Texture* grayscale = SDL_CreateTextureFromSurface(renderer, surface);

    black_white_level(surface);

    SDL_Texture* blackwhite = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    // - Dispatch the events.
    event_loop(renderer, grayscale, blackwhite);
    // - Destroy the objects.
    SDL_DestroyTexture(grayscale);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;


}
