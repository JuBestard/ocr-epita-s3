#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include <stdlib.h>
#include <stdio.h>
#include "err.h"
void load_grid(char* file, int grid[9][9]);
void save_grid(int grid[9][9], char* file);
#endif
