#include "detection_grid/hough/hough.h"
#include "image_process/toolbox/pixels_op.h"
#include "tools.h"
#include "math.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>

#define RES 10 //ressemblance a XX pixels pres
#define pi 3.14159265359
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

int checkRes(int* finX, int* finY, int maxFin, int x, int y)
{
    /*int xMax = x1+RES;
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
    return 1;*/

    for(int i = 0; i < maxFin; i++)
    {
        if(finX[i] - RES < x && x < finX[i]+RES && 
                finY[i] - RES < y && y < finY[i]+RES)
            return 0;
    }
    return 1;
}

int checkHorVertTmp(SDL_Surface* s, int x1, int y1, int x2, int y2)
{
    int x = x2 - x1;
    int y = y2 - y1;
    double aHor = angle(20, 0, x, y);
    double aVert = angle(0, 20, x, y);
    if((aHor <= 0.01 && aVert >= 1.51) || (aHor >= 1.51 && aVert <= 0.01))
    {
        drawLine(s, x1, y1, x2, y2, SDL_MapRGB(s->format, 0, 255, 0));
        return 1;
    }
    return 0;
}
int checkHorVert(int x, int y)
{
    double aHor = angle(20, 0, x, y);
    double aVert = angle(0, 20, x, y);
    if((aHor <= 0.01 && aVert >= 1.51) || (aHor >= 1.51 && aVert <= 0.01))
        return 1;
    return 0;
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
    SDL_Surface* gridlines = SDL_ConvertSurface(s, s->format, SDL_SWSURFACE);

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
                drawLine(gridlines, x1, y1, x2, y2, SDL_MapRGB(s->format, 255, 0, 0));
                checkHorVertTmp(gridlines, x1, y1, x2, y2);
                maxA++;
                flagFirst = 0;
            }
        }
    }

    SDL_SaveBMP(gridlines, "out/gridlines.bmp");
    SDL_FreeSurface(gridlines);
    int* intX = malloc(sizeof(int) *width*height*2);
    int* intY = malloc(sizeof(int) * width*height*2);

    for(int i = 0; i < maxA; i++) //1er vecteur ligne AB
    {
        int ux = accX2[i] - accX1[i];
        int uy = accY2[i] - accY1[i];

        for(int j = 0; j < maxA; j++) //2e vecteur ligne CD
        {
            int vx = accX2[j] - accX1[j];
            int vy = accY2[j] - accY1[j];

            double a = angle(ux, uy, vx, vy);
            if(checkHorVert(vx, vy) && a > 1.47 && a < 1.67)
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

    int* finX = malloc(sizeof(int) * width*height*2);
    int* finY = malloc(sizeof(int) * width*height*2);
    for(int i = 0; i < maxInt; i++)
    {
            if(checkRes(finX, finY, maxF, intX[i], intY[i]))
            {
                finX[maxF] = intX[i];
                finY[maxF] = intY[i];
                maxF++;
            }
    }
    free(intX);
    free(intY);
    int xbd = s->w/2, ybd = s->h/2;
    int xbg = s->w/2, ybg = s->h/2;
    int xhd = s->w/2, yhd = s->h/2;
    int xhg = s->w/2, yhg = s->h/2;
    for(int i = 0; i < maxF; i++)
    {
        int x = finX[i], y = finY[i];
        if(x <= xbg && y >= ybg && y)
        {
            xbg = x;
            ybg = y;
        }
        else if (x >= xbd && y >= ybd)
        {
            xbd = x;
            ybd = y; 
        }
        else if (x >= xhd && y <= yhd)
        {
            xhd = x;
            yhd = y;
        }
        else if (x <= xhg && y <= yhg)
        {
            xhg = x;
            yhg = y;
        }
    }
    free(finX);
    free(finY);
    
    int longueur = sqrt((xbd-xbg)*(xbd-xhg)+(ybd-ybg)*(ybd-ybg));
    SDL_Rect src;
    src.x = xbg;
    src.y = ybg-longueur;
    src.w = longueur;
    src.h = longueur;
    printf("%s (%i,%i)\n", "Haut droite", xhd, yhd);
    printf("%s (%i,%i)\n", "Haut gauche", xhg, yhg);
    printf("%s (%i,%i)\n", "bas droite", xbd, ybd);
    printf("%s (%i,%i)\n", "bas gauche", xbg, ybg);
    SDL_Surface* otsu = load_image("out/out.bmp");
    SDL_Surface* out = SDL_CreateRGBSurface(0, longueur, longueur, 16, 0, 0, 0, 0);
    SDL_BlitSurface(otsu, &src, out, NULL);
    SDL_SaveBMP(out, "out/grid.bmp");
    /*
    trouver l'angle le plus petit entre vecteur vertical et tout les vecteurs -> angle de rotate
    decoupe l'image
    */
    SDL_FreeSurface(out);
    SDL_FreeSurface(otsu);
}
