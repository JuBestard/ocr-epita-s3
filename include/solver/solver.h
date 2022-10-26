#ifndef SOLVER_H
#define SOLVER_H

typedef struct Tuple
{
    int x;
    int y;
} Tuple;

int IsBoardValid(int board[9][9]);
int IsSolved(int board[9][9]);
Tuple FindEmpty(int board[9][9]);
int Solve(int board[9][9]);

void printGrid(int board[9][9]);

#endif
