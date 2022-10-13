#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int decompose(const char* file, float* rValue)
{
    SDL_Surface* temp = IMG_Load(file);
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(temp);

    Uint8 r,g,b;
    Uint32* pixels = surface -> pixels;
    int w = surface -> w;
    int h = surface -> h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    rValue = calloc(w*h, sizeof(float));
    for(int i=0; i<w*h;i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        float average = ((r+g+b)/3)/256;
        rValue[i] = average;
    }
    return h;
}

int main(int argc, char** argv)
{
    if (argc == 2)
        return EXIT_FAILURE;
    float* tab = NULL;
    int c = decompose(argv[1], tab);
    for(int i=0; i<c*c;i++)
    {
        if(i/c ==0 && i!=0)
            printf("\n");
        printf("%f | ",tab[i]);
    }
    return EXIT_SUCCESS;
}
