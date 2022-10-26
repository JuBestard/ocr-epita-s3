#include "solver/solver.h"
#include "solver/load_save.h"
#include "err.h"
#include <stdio.h>

int  RowCheck(int val, int board[N][N], int row)
// return 1 if the value is not already on the row
{
    for (int col=0; col < 9; col++)
        if (board[row][col] == val)
            return 0;
    return 1;
}

int ColCheck(int val, int board[N][N], int col)
// return 1 if the value is not already on the columns
{
    for (int row=0; row < 9; row++)
        if (board[row][col] == val)
            return 0;
    return 1;
}

int BoxCheck(int val, int board[N][N], int row, int col)
// return 1 if the value is not already on the box (3x3)
{
    int i = row-(row%n), j = col-(col%n);  // ou encore : _i = 3*(i/3), _j = 3*(j/3);
    for (row = i; row < i+n; row++)
        for (col = j; col < j+n; col++)
            if (board[row][col] == val)
                return 0;
    return 1;
}

int Solve(int board[N][N], int position)
{
    if (position == N*N)
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

int main()
{
    int grid[9][9];

    load_grid("grid_00", grid);
    Solve(grid);
    save_grid(grid, "grid_00.result");
    return 0;
}
