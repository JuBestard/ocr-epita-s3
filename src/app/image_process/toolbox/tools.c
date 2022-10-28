#include "tools.h"

SDL_Surface* load_image(const char* file)
{
    SDL_Surface* tmp = IMG_Load(file);
    SDL_Surface* off = SDL_ConvertSurfaceFormat(tmp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(tmp);
    return off;
}

void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
