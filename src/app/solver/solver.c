#include "solver.h"

int IsBoardValid(int board[][9])
{
    for(int i = 1; i <= 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int col = 0;
            int lig = 0;
            for(int k = 0; k < 9; k++)
            {
                if(board[j][k] == i)
                    lig += 1;
                if(board[k][j] == i)
                    col += 1;
                if(col > 1 || lig > 1)
                    return 1;
            }
        }

        for(int x = 0; x < 3; x++)
        {
            for(int y = 0; y < 3; y++)
            {
                int grid = 0;
                for(int j = 0; j < 3; j++)
                {
                    for(int k = 0; k < 3; k++)
                    {
                        if(board[3 * x + j][3 * y + k] == i)
                            grid += 1;
                    }
                }
                if(grid > 1)
                    return 0;
            }
        }
    }

    return 1;
}

int IsSolved(int board[9][9])
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            if(board[i][j] == 0)
                return 0;
        }
    }
    return 1;
}

