#include "detection_grid/hough/hough.h"
#include "image_process/toolbox/pixels_op.h"
#include "tools.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define RES 100 //ressemblance a XX pixels pres

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

double angle(int x1, int y1, int x2, int y2)
{
    double aNum = x1*x2 +y1*y2;
    double aDen = sqrt(x1*x1 + y1*y1) * sqrt(x2*x2 + y2*y2);
    if(aNum < 0)
        aNum = -aNum;
    if(aDen == 0)
        return 1.5798;
    return acos(aNum/aDen);
}

int checkRes(int x1, int y1, int x2, int y2)
{
    int xMax = x1+RES;
    int xMin = x1-RES;
    int yMax = y1+RES;
    int yMin = y1-RES;
    if(x2 > xMax)
        return 0;
    if(x2 < xMin)
        return 0;
    if(y2 > yMax)
        return 0;
    if(y2 < yMin)
        return 0;
    return 1;
}

void hough(SDL_Surface* s)
{
    int width = s->w;
    int height = s->h;
    int rho = sqrt(width*width + height*height);
    int theta = 180;

    //initialise accumulator array
    int acc[rho][theta];
    
    int* accX1 = malloc(sizeof(int) * width*height);
    int* accY1 = malloc(sizeof(int) * width*height);
    int* accX2 = malloc(sizeof(int) * width*height);
    int* accY2 = malloc(sizeof(int) * width*height);

    int* intX = malloc(sizeof(int) * width*height);
    int* intY = malloc(sizeof(int) * width*height);

    int* finX = malloc(sizeof(int) * width*height);
    int* finY = malloc(sizeof(int) * width*height);

    int maxA = 0;
    int maxInt = 0;
    int maxF = 0;

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

    SDL_Surface* output = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

    for(int r = 0; r < rho; r++)
    {
        for(int t = 0; t < theta; t++)
        {
            if(acc[r][t] > accMax*0.4)
            {
                for(int y = 0; y <height; y++)
                {
                    for(int x = 0; x < width; x++)
                    {
                        if(r == abs((int) floor(x * cos(t * M_PI/180) + y*sin(t * M_PI/180))))
                        {
                            //putpixel(output, x, y, linePixel1);
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
                accX1[maxA] = x1;
                accY1[maxA] = y1;
                accX2[maxA] = x2;
                accY2[maxA] = y2;
                maxA++;
                //drawLine(output, x1, y1, x2, y2, linePixel2);
                flagFirst = 0;
            }
        }
    }
    for(int i = 0; i < maxA; i++) //1er vecteur ligne AB
    {
        int ux = accX2[i] - accX1[i];
        int uy = accY2[i] - accY1[i];

        for(int j = 0; j < maxA; j++) //2e vecteur ligne CD
        {
            int vx = accX2[j] - accX1[j];
            int vy = accY2[j] - accY1[j];

            double a = angle(ux, uy, vx, vy);

            if(a > 1.47 && a < 1.67)
            {
                int det = uy*vx - vy*ux;
                int x = uy*accX1[i] - ux*accY1[i];
                int y = vy*accX1[j] - vx*accY1[j];

                intX[maxInt] = abs((vx*x - ux*y)/det);
                intY[maxInt] = abs((uy*x - vy*x)/det);
                maxInt++;
            }
        }
    }
    free(accX1);
    free(accY1);
    free(accX2);
    free(accY2);
    for(int i = 0; i < maxInt; i++)
    {
        for(int j = 0; j < maxInt; j++)
        {
            if(checkRes(intX[i], intY[i], intX[j], intY[j]))
            {
                printf("%i -- %i\n", maxF, j);
                finX[maxF] = intX[j];
                finY[maxF] = intY[j];
                maxF++;
            }
        }
    }
    free(intX);
    free(intY);
    int xhd = finX[0], yhd = finY[0];
    int xbd = finX[0], ybd = finY[0];
    int xhg = finX[0], yhg = finY[0];
    int xbg = finX[0], ybg = finY[0];

    for(int i = 1; i < maxF; i++)
    {
        int x = finX[i], y = finY[i];
        if(x <= xhg && y >= yhg)
        {
            xhg = x;
            yhg = y;
        }
        else if (x >= xhd && y >= yhd)
        {
            xhd = x;
            yhd = y; 
        }
        else if (x >= xbd && y <= ybd)
        {
            xbd = x;
            ybd = y;
        }
        else if (x <= xbg && y <= ybg)
        {
            xbg = x;
            ybg = y;
        }
    }
    putpixel(output, xhg, yhg, linePixel1);
    putpixel(output, xhd, yhd, linePixel1);
    putpixel(output, xbg, ybg, linePixel1);
    putpixel(output, xbd, ybd, linePixel1);
    printf("%s (%i, %i)\n", "haut gauche:", xhg, yhg);
    printf("%s (%i, %i)\n", "haut droite:", xhd, yhd);
    printf("%s (%i, %i)\n", "bas gauche:", xbg, ybg);
    printf("%s (%i, %i)\n", "bas droite:", xbd, ybg);
    drawLine(output, xhg, yhg, xhd, yhg, linePixel1);
    drawLine(output, xhg, yhg, xbg, ybg, linePixel1);
    drawLine(output, xbg, ybg, xbd, ybd, linePixel1);
    drawLine(output, xbd, ybd, xhd, yhd, linePixel1);
    SDL_SaveBMP(output, "hough.bmp");
    free(finX);
    free(finY);
    /*
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
