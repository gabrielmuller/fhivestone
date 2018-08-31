#include "heuristics.h"

#define DISPLAY

typedef struct Board Board;

Board* create_board (int size);
void play_row (row_t* row, int pos, int piece);
void play_orientation (row_t* row, int x, int y, int piece);
void play_board (Board* board, int x, int y, int piece);

#ifdef DISPLAY
void print_board (Board* board);
#endif
