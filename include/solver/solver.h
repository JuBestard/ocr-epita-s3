#ifndef SOLVER_H
#define SOLVER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_rect.h>
/*typedef struct Tuple
{
    int x;
    int y;
} Tuple;*/

int Solve(int board[9][9], int position);
void render_grid(int boardunsolved[9][9], int boardsolved[9][9]);

#endif
