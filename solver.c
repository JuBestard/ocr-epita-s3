#include <stdio.h>

#define True 1
#define False 0
const int N = 9;
const int n = 3;
const int EMPTY = 0;

//convert the . into a 0


int RowCheck(int board[N][N], int row, int val)
//check if a value is in the given row
{
    for (int col = 0; col < N; col++)
    {
        if (board[row][col] == val)
            return True;
        return False;
    }
}

int ColCheck(int board[N][N], int col, int val)
//check if a value is in the given col
{
    for (int row = 0; row < N; row++)
    {
        if (board[row][col] == val)
            return True;
        return False;
    }   
}

int BoxCheck(int board[N][N], int boxStartRow, int boxStartCol, int val)
//check if a value is in the given box
{
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col ++)
        {
            if (board[row + boxStartRow][col + boxStartCol] == val)
                return True;
        }
    }
    return False;
}

int IsValid(int board[N][N], int row, int col, int val)
//check if the number is a valid entry in the case
{
    if (RowCheck(board, row, val) == False && ColCheck(board, col, val) == False
            && BoxCheck(board, row - row % n, col - col % n, val) == False 
            &&  board[row][col] == EMPTY)
        return True;
    return False;
}

int FindEmpty(int board[N][N])
//find the first cell with no value
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; row++)
        {
            if(board[row][col] = EMPTY)
                return True;
        }
    }
    return False;
}

int Solve(int board[N][N])
//main function to solve the board
{
    //init variables
    int row, col;

    //check if there is empty cell left or not
    if (FindEmpty == False)
        return True;

    //check each value from 1 to 9 and if it's a correct move
    for (int val = 1; val <= N; val ++)
    {
        if(IsValid(board, row, col, val))
        {
            board[row][col] = val;

            if(SolveSudoku(board))
                return True;

            board[row][col] = val;
        }
    }
    return False;
}

//####temporary main#####

void printGrid(int board[N][N])
{
    for (int row = 0; row < N; row ++)
    {
        for (int col = 0; col <N ; col ++)
        {
            printf("%2d", board[row][col]);
        }
        printf("\n");
    }
}

int main()
{
    int grid[N][N] = { { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
                       { 5, 2, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
                       { 0, 0, 3, 0, 1, 0, 0, 8, 0 },
                       { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
                       { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
                       { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
                       { 0, 0, 5, 2, 0, 6, 3, 0, 0 } };

    Solve(grid);
    printGrid(grid);
    return 0;
}
