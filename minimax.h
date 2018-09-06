#ifndef MINIMAX_H
#define MINIMAX_H

#include "board.h"

int play_minimax (Board* board, int player);
int minimax (Board* board, Pos* queue, int depth, int alpha, int beta, int minimizing);
#endif
