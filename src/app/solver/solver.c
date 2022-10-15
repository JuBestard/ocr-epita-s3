#include "solver.h"

int IsBoardValid(int grid[][9])
{
    for(int i = 1; i <= 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            int col = 0;
            int lig = 0;
            int grid = 0;
            for(int k = 0; k < 9; k++)
            {
                if(grid[0][k] == i)
                    lig += 1;
                if(grid[k][k] == i)
                    col += 1;
                if(col > 1 || lig > 1)
                    return 1;
            }
        }
    }
}
