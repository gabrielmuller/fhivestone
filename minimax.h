#ifndef MINIMAX_H
#define MINIMAX_H

#include "board.h"

int play_minimax (Board* board, int player);
int minimax (Board* board, int depth, int alpha, int beta, int minimizing, int last_x, int last_y);
#endif
