#include "detection_grid/hough/hough.h"
#include "image_process/toolbox/pixels_op.h"
#include "tools.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>

void drawLine(SDL_Surface* s, int x1, int y1, int x2, int y2, Uint32 pixel)
{
    double x = x2 - x1;
    double y = y2 - y1;
    double length = sqrt(x*x + y*y);
    double addx = x / length;
    double addy = y / length;
    x = x1;
    y = y1;
    for(int i = 0; i < length; i++)
    {
        putpixel(s, x, y, pixel);
        x += addx;
        y += addy;
    }
}

void hough(SDL_Surface* s)
{
    int width = s->w;
    int height = s->h;
    int rho = sqrt(width*width + height*height);
    int theta = 180;

    //initialise accumulator array
    int acc[rho][theta];
    
    int vector[width][height];
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            for(int k = 0; k < 2; k++)
                vector[i][j] = 0;
        }
    }
    int flagFirst = 0;
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    for (int i = 0; i < rho; i++)
    {
        for (int j = 0; j < theta; j++)
            acc[i][j] = 0;
    }
    Uint32 pixel;
    Uint8 r, g, b;
    int accMax = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            pixel = getpixel(s, x, y);
            SDL_GetRGB(pixel, s->format, &r, &g, &b);

            //if white
            if (r >= 155)
            {
                for (int t = 0; t < 180;t++)
                {
                    int p = abs((int) floor(x*cos(t * M_PI/180) + y*sin(t * M_PI/180)));
                    acc[p][t]++;
                }
            }
        }
    }
    for(int r = 0; r < rho; r++)
    {
        for(int t = 0; t < theta; t++)
        {
            if(acc[r][t] > accMax)
                accMax = acc[r][t];
        }
    }
    printf("%i\n", accMax);
    Uint32 linePixel1 = SDL_MapRGB(s->format, 255, 0, 0);
    Uint32 linePixel2 = SDL_MapRGB(s->format, 0, 255, 0);
    Uint32 linePixel3 = SDL_MapRGB(s->format, 0, 0, 255);
    SDL_Surface* bw = load_image("out.bmp");
    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    for(int r = 0; r < rho; r++)
    {
        for(int t = 0; t < theta; t++)
        {
            if(acc[r][t] > accMax*0.45)
            {
                for(int y = 0; y <height; y++)
                {
                    for(int x = 0; x < width; x++)
                    {
                        if(r == abs((int) floor(x * cos(t * M_PI/180) + y*sin(t * M_PI/180))))
                        {
                            putpixel(output, x, y, linePixel1);
                            if(flagFirst)
                            {
                                x2 = x;
                                y2 = y;
                            }
                            else
                            {
                                flagFirst = 1;
                                x1 = x;
                                y1 = y;
                            }
                        }
                    }
                }
                //vector[x1][y1][0] = x2;
                //vector[x1][y1][1] = y2;
                flagFirst = 0;
            }
        }
    }
    /*for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(vector[x][y][0] != 0 && vector[x][y][1] != 0)
            {
                printf("%i --- %i\n", vector[x][y][0], vector[x][y][1]);
                drawLine(output, x, y, vector[x][y][0], vector[x][y][1], linePixel3);
            }
        }
    }
    SDL_SaveBMP(output, "hough.bmp");
    /*
        on prend le premier point et denier pour creer un vecteur (x1- x2, y1-y2)
        flag pour le premier
            ligne entre 1er et dernier
    recuper intersection vecteur
    regarder les angles entre les vecteurs 2 : > 30 et < 150, j'garde l'intersection
    clean les intersections
    trouver l'angle le plus petit entre vecteur vertical et tout les vecteurs -> angle de rotate
    pour chaque point si, il a 10 pixel ete rencontre -> skip sinon stock comme point interessant
    recuperer les 4 point extremes
    decoupe l'image

    calculer ratio largeur/9

    */
    SDL_FreeSurface(output);
}
