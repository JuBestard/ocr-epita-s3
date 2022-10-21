#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void decompose(SDL_Surface* surface, float* rValue)
{
    Uint8 r,g,b;
    Uint32* pixels = surface -> pixels;
    int w = surface -> w;
    int h = surface -> h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    for(int i=0; i<w*h;i++)
    {
        SDL_GetRGB(pixels[i], format, &r, &g, &b);
        float average = ((r+g+b)/3)/256;
        rValue[i] = average;
    }
}

int main(int argc, char** argv)
{
    if (argc == 2)
        return EXIT_FAILURE;

    SDL_Surface* temp = IMG_Load(argv[1]);
    if (temp ==NULL)
	    errx(EXIT_FAILURE,"%s\n",SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(temp);

    int w = surface -> w;
    int h = surface -> h;
    float* tab = calloc(w*h, sizeof(float));
    decompose(surface, tab);
    for(int i=0; i<h*w;i++)
    {
        if(i/w==0 && i!=0)
            printf("\n");
        printf("%f | ",tab[i]);
    }
    return EXIT_SUCCESS;
}
