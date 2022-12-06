#include "solver/solver.h"
#include "solver/load_save.h"
#include "SDL2/SDL_surface.h"
#include "toolbox.h"
#include "err.h"
#include "string.h"
#include <SDL2/SDL_rect.h>
#include <stdio.h>


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

void render_grid(int grid[9][9], char* path)
{
    SDL_Surface* s = load_image(path);

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            SDL_Rect src = {75, 75, 0, 0};
            SDL_Rect dst = {75, 75, i*75, j*75};
        }
    }

    SDL_SaveBMP(s, "init.bmp");
}

int main(int argc, char** argv)
{
    if(argc != 2)
        errx(1, "usage : ./solver <path-to-grid>");
    int grid[9][9];
    load_grid(argv[1], grid);
    render_grid(grid, "empty.bmp");
    Solve(grid, 0);
    save_grid(grid, strcat(argv[1], ".result"));
    render_grid(grid, "init.bmp");
    return 0;
}
