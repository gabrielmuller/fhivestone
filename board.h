#ifndef BOARD_H
#define BOARD_H

#include "heuristics.h"

#define DISPLAY
#define WIN 900000
typedef struct Board Board;

extern int* htable;
Board* create_board (int size);
int play_row (row_t* row, int pos, int piece);
int play_orientation (row_t* row, int x, int y, int piece);
void play_board (Board* board, int x, int y, int piece);
int utility (Board* board);

#ifdef DISPLAY
void print_board (Board* board);
#endif

#endif
