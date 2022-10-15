#include "load_save.h"
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
                    grid[i][j] = charActuel;
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
