#ifndef BOARD_H
#define BOARD_H

#include "heuristics.h"
#include <stdio.h>
#include <string.h>

#define DISPLAY
#define WIN 900000

typedef struct {
    row_t* horizontal; // tabuleiro normal
    row_t* vertical;   // tabuleiro transposto
    row_t* descending; // diagonal cima-esquerda para baixo-direita
    row_t* ascending;  // diagonal baixo-esquerda para cima-direita
    int hval;          // valor heurístico
    int size;          // dimensões
    int diag_rows;
    int last_x;
    int last_y;
} Board;

extern int* htable;
Board* create_board ();
Board* copy_board (Board* other);
int select_affected(row_t row, int pos);
int play_row (row_t* row, int pos, int piece);
int play_orientation (row_t* row, int x, int y, int piece);
int min (int x, int y);
int play_board (Board* board, int x, int y, int piece);
int utility (Board* board);

#ifdef DISPLAY
void print_board (Board* board);
#endif

#endif
