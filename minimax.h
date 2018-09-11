#ifndef MINIMAX_H
#define MINIMAX_H

#include "board.h"

int play_minimax (Board* board, int player, int effort, int depth);
int minimax (Board* board, Pos* queue, int effort, int depth, int alpha, int beta, int minimizing, unsigned long long int* counter);
#endif
