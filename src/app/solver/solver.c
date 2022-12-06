#include "solver/solver.h"
#include "solver/load_save.h"
#include "tools.h"
#include "image_process/toolbox/pixels_op.h"
#include "err.h"
#include "string.h"
#include <SDL2/SDL_pixels.h>
#include <stdio.h>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_rect.h>

int  RowCheck(int val, int board[9][9], int row)
// return 1 if the value is not already on the row
{
    for (int col=0; col < 9; col++)
        if (board[row][col] == val)
            return 0;
    return 1;
}

int ColCheck(int val, int board[9][9], int col)
// return 1 if the value is not already on the columns
{
    for (int row=0; row < 9; row++)
        if (board[row][col] == val)
            return 0;
    return 1;
}

int BoxCheck(int val, int board[9][9], int row, int col)
// return 1 if the value is not already on the box (3x3)
{
    int i = row-(row%3), j = col-(col%3);
    for (row = i; row < i+3; row++)
    for (col = j; col < j+3; col++)
            if (board[row][col] == val)
                return 0;
    return 1;
}

int Solve(int board[9][9], int position)
{
    if (position == 9*9)
        return 1;

    int row = position/9, col = position%9;

    if (board[row][col] != 0)
        return Solve(board, position+1);

    for (int val=1; val <= 9; val++)
    {
        if (RowCheck(val ,board, row)== 1
                && ColCheck(val,board,col) == 1
                && BoxCheck(val,board,row,col))
        {
            board[row][col] = val;

            if (Solve(board, position+1)== 1)
                return 1;
        }
    }
    board[row][col] = 0;

    return 0;
}

void render_grid(int gridunsolved[9][9], int grindsolved[9][9])
{
    SDL_Surface* s = load_image("given_grid/visu/empty.jpeg");
    int x = 15, y = 10;
    Uint32 red = SDL_MapRGB(s->format, 255, 0, 0);
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            char buffer[30];
            snprintf(buffer, 30, "given_grid/visu/numbers/%i.png", grindsolved[i][j]);
            SDL_Surface* number = load_image(buffer);
            if(gridunsolved[i][j] != grindsolved[i][j])
            {
                for(int px = 0; px < 75; px++)
                {
                    for(int py = 0; py < 75; py++)
                    {
                        Uint8 r, g, b;
                        Uint32 pixel = getpixel(number, px, py);
                        SDL_GetRGB(pixel, number->format, &r, &b, &g);
                        if(r != 255)
                            putpixel(number, px, py, red);

                    }
                }
            }
            SDL_Rect dst = {x, y, 75, 75};
            SDL_BlitSurface(number, NULL, s, &dst);
            free(number);
            x += 100;
        }
        x = 15;
        y += 100;
    }
    SDL_SaveBMP(s, "out/resolve.bmp");
    free(s);
}

int main(int argc, char** argv)
{
    if(argc != 2)
        errx(1, "usage : ./solver <path-to-grid>");
    int gridsolved[9][9];
    int gridunsolved[9][9];
    load_grid(argv[1], gridunsolved);
    load_grid(argv[1], gridsolved);
    Solve(gridsolved, 0);
    save_grid(gridsolved, strcat(argv[1], ".result"));
    render_grid(gridunsolved, gridsolved);
    return 0;
}
