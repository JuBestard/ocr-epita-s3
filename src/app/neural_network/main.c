#include "decompose.h"

int main(int argc, char** argv)
{
    if (argc != 2)
	    return EXIT_FAILURE;
    SDL_Surface* temp = SDL_LoadBMP(argv[1]);
    if (temp ==NULL)
	    errx(EXIT_FAILURE,"%s\n",SDL_GetError());
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(temp);

    int w = surface -> w;
    int h = surface -> h;
    float* tab = malloc(w*h*sizeof(float));
    decompose(surface, tab);
    for(int i=0; i<h*w;i++)
    {
        if(i%w==0 && i!=0)
            printf("\n");
        printf("%.f | ",tab[i]);
    }
    printf("\n");

    SDL_FreeSurface(surface);
    return EXIT_SUCCESS;
}
