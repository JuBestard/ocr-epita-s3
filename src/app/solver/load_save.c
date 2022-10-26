#include "solver/load_save.h"
#include "solver/solver.h"
#include <err.h>
#include <stdio.h>

void load_grid(char* path, int grid[9][9])
{
    FILE* file = NULL;
    file = fopen(path, "r");
    char charActuel = '0';

    if(file != NULL)
    {
        int i = 0;
        while(i < 9)
        {
            int j = 0;
            while(j < 9)
            {
                charActuel = fgetc(file);
                if(charActuel != '\n' && charActuel != ' ')
                {
                    if(charActuel == '.')
                        grid[i][j] = 0;
                    else
                        grid[i][j] = charActuel - '0';
                    j++;
                }
            }
            i++;
        }
        fclose(file);
    }
    else
    {
        errx(1, "Connot open file");
    }
}

void save_grid(int grid[9][9], char* path)
{
    FILE* file = NULL;
    file = fopen(path, "w");
    
    if(file != NULL)
    {
        for(int i = 0; i < 9; i++)
        {
            if(i == 3 || i == 6)
                fputc('\n',file);
            for(int j = 0; j < 9; j++)
            {
                if(j == 3 || j == 6)
                    fputc(' ',file);
                fputc((char)(grid[i][j] + 48), file);
            }
            fputc('\n', file);
        }
        fclose(file);
    }
    else
    {
        errx(1, "Cannot open file");
    }
}
