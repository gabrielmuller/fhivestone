#ifndef MINIMAX_H
#define MINIMAX_H

#include "board.h"

typedef unsigned long long int counter_t;

int play_minimax (Board* board, int player, int effort, int depth);
int minimax (Board* board,
             Pos* queue,
             int effort,
             int depth,
             int alpha,
             int beta,
             int minimizing,
             counter_t* counter
             );
#endif
