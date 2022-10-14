#include "tools.h"
#include "err.h"

void event_loop(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Texture* blackwhite)
{
   SDL_Event event;
   SDL_Texture* t = texture;
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
                if(event.key.keysym.sym == SDLK_t)
                {
                    t = t == texture ? blackwhite : texture;
                    draw(renderer, t);
                }
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    return;
                break;
       }
   }
}

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
    SDL_Surface* surface = load_image(argv[1]);

    // Gets the width and the height of the texture.
    int w = surface->w;
    int h = surface->h;
    SDL_PixelFormat* format =surface-> format;

    // - Resize the window according to the size of the image.
    SDL_SetWindowSize(window, w, h);

    apply_grayscale(surface, format);
    // - Create a new texture from the grayscale surface.
    SDL_Texture* grayscale = SDL_CreateTextureFromSurface(renderer, surface);
    invert(surface,format, 1);

    contrast(surface, format);
    invert(surface, format, 0);
    SDL_Texture* blackwhite = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    // - Dispatch the events.
    event_loop(renderer, grayscale, blackwhite);
    // - Destroy the objects.
    SDL_DestroyTexture(grayscale);
    SDL_DestroyTexture(blackwhite);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;


}