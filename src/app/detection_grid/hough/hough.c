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
double Convert(int degree)
{
    return degree * (pi / 180);
}
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

void houghTransformation(SDL_Surface* image)
{
    int width = image->w;
    int height = image->h;
    int diagonale = floor(sqrt((double) (width * width + height * height)));

    // there will be 180 teta since we go from 0 include to 180 excluded
    // there will be diagonale rho
    int A[diagonale][180];
    for (int i = 0; i < diagonale; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            A[i][j] = 0;
        }
    }

    for (double x = 10; x < width-10; x++)
    {
        for (double y = 10; y < height-10; y++)
        {
            Uint32 pixel = getpixel(image, floor(x), floor(y));
            Uint8 r,g,b;
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r > 10)
            {
                for (int teta = 0; teta < 180; teta++)
                {
                    double tetaRad = Convert(teta);
                    //printf("floor(cos) = %i  teta = %i\n", rho, teta);
                    //if (x * (cos(tetaRad)) - y * (sin(tetaRad) + x * cos(tetaRad) + y * sin(tetaRad)) == 0)
                    {
                        
                        int rho = abs((int) floor(x * cos(tetaRad) + y * sin(tetaRad)));
                        //printf("rho = %i\n", rho);
                        A[rho][teta] += 1;
                        //printf("long thing = %f cos(rho) = %i  cos(tetaRad) = %f  sin(tetaRad) = %f  x = %f  y = %f\n", x * (cos(tetaRad)) - y * (sin(tetaRad)) + rho, rho, cos(tetaRad), sin(tetaRad), x, y);
                        //printf("hello my bwuda 2\n");
                            /*for (int i = -1; i < 2; i++)
                            {
                                for (int j = -1; j < 2; j++)
                                {   
                                    if (rho + i >= 0 && rho + i < diagonale && teta + j >= 0 && teta + j < 180)
                                    {
                                        //printf("hello my bwuda 3\n");
                                        A[rho + i][teta + j] += 1;
                                    }
                                }
                            }*/
                    }
                }
            }
        }
    }
    
    // EDGE DETECTION DONE

    SDL_Surface* houghSpace = SDL_CreateRGBSurface(0, 180, diagonale, 32, 0, 0, 0, 0);
    SDL_Surface* intersection = load_image("out/out.bmp");

    int Line =0;
    for (int j = 0; j < 180; j+=90)
    {
        for (int i = 0; i < diagonale-29; i+=30)
        {
	    	int indexk = 0;
		int imax = 0;
		int max = 0;
                for (int k =0 ; k <= 30; k++)
                {
                        if (i+k >= 0 && i+k < diagonale)
                        {
				if( A[i+k][j] >= max)
				{
                            		max = A[i+k][j];
                            		indexk = i+k;
					A[imax][k]=-1;
					imax = i+k;
				}
				else
				{
					A[i+k][j] = -1; 
				}
                        }
                }

            //printf("this : %i\n", A[i][j]);
            if (A[indexk][j] >275)
            {
                    /*for (int x = 0; x < width; x++)
                    {
                        printf("teta = %i\n", j);
                        if (j == 0)
                        {
                            int y = (int) (indexk - x * pi / 2);
                            
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (y < 1000 && y >= 0)
                            {
                                put_pixel(image, x, y, pix);
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                        if (j == 90)
                        {
                            int y = (int) (indexk - x * cos(Convert(j)) / sin(Convert(j)));
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 34, 34);
                            if (y < 1000 && y >= 0)
                            {
                                put_pixel(image, x, y, pix);
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                        
                    }*/
                    if (j >= 90 && j <= 90)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            int y = (int) (indexk - x * cos(Convert(j)) / sin(Convert(j)));
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, 33, 33);
                            if (y < height && y >= 0)
                            {
				Uint32 pixel = getpixel(image, floor(x), floor(y));
            			Uint8 r,g,b;
            			SDL_GetRGB(pixel, image->format, &r, &g, &b);

				if(r == 178)
				{
					putpixel(intersection,x,y,pix);
				}

                                putpixel(image, x, y, pix);
				Line +=1;

                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                    }
                    if (j == 0)
                    {
                        for (int y = 0; y < height; y++)
                        {
                            int x = (int) (indexk - y * sin(Convert(j)) / cos(Convert(j)));
                            //printf("x = %i    y = %i\n", x, y);
                            Uint32 pix = SDL_MapRGB(image->format, 178, j, j);
                            if (x < width && x >= 0)
                            {
				Uint32 pixel = getpixel(image, floor(x), floor(y));
            			Uint8 r,g,b;
            			SDL_GetRGB(pixel, image->format, &r, &g, &b);

				if(r == 178)
				{
					putpixel(intersection,x,y,pix);
				}
                                putpixel(image, x, y, pix);
				Line += 1;
                            }
                            else
                            {
                                //printf("y over 1000 : %i\n", y);
                            }
                        }
                    }
                //int x = i * floor(cos(j));
                //int y = i * floor(sin(j));
            }
        }
    }
    //printf("%i",Line);
    SDL_SaveBMP(houghSpace, "out/houghSpace.bmp");
    SDL_SaveBMP(image, "out/muchachos.bmp");
    SDL_SaveBMP(intersection, "out/intersection.bmp");
    // squaredetect();
    SDL_FreeSurface(intersection);
    SDL_FreeSurface(image);
    SDL_FreeSurface(houghSpace);
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
            if (r >= 100)
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
            if(acc[r][t] > accMax*0.44)
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
                maxA++;
                flagFirst = 0;
            }
        }
    }

    SDL_SaveBMP(gridlines, "out/gridlines.bmp");
    SDL_FreeSurface(gridlines);
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
    free(finX);
    free(finY);
    
    int longueur = sqrt((xhd-xhg)*(xhd-xhg)+(yhd-yhg)*(yhd-yhg));
    int hauteur = sqrt((xbg-xhg)*(xbg-xhg)+(ybg-yhg)*(ybg-yhg));
    SDL_Rect src;
    src.x = xbg;
    src.y = ybg;
    src.w = longueur;
    src.h = hauteur;

    SDL_Surface* otsu = load_image("out/out.bmp");
    SDL_Surface* out = SDL_CreateRGBSurface(0, longueur, hauteur, 16, 0, 0, 0, 0);
    SDL_BlitSurface(otsu, &src, out, NULL);
    SDL_SaveBMP(out, "out/grid.bmp");
    /*
    trouver l'angle le plus petit entre vecteur vertical et tout les vecteurs -> angle de rotate
    decoupe l'image
    */
    SDL_FreeSurface(out);
    SDL_FreeSurface(otsu);
}
