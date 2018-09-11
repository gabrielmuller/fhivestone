#ifndef BOARD_H
#define BOARD_H

#include "heuristics.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    /*
     * Struct tabuileiro.
     * Usada tanto para representar o tabuleiro do jogo de fato quanto
     * o tabuleiro no qual são feitas simulações.
     */
    row_t* horizontal; // tabuleiro normal
    row_t* vertical;   // tabuleiro transposto
    row_t* descending; // diagonal cima-esquerda para baixo-direita
    row_t* ascending;  // diagonal baixo-esquerda para cima-direita
    int hval;          // valor heurístico
    int size;          // dimensões
    int diag_rows;
} Board;

typedef struct {
    /*
     * Possível jogada.
     * Armazena sua posição e valor heurístico.
     */
    int x;
    int y;
    int hval;
} Pos;

extern int* htable;
Board* create_board ();
void free_board (Board* board);
Board* copy_board (Board* other);
int select_affected(row_t row, int pos);
int play_row (row_t* row, int pos, int piece);
int play_orientation (row_t* row, int x, int y, int piece);
int min (int x, int y);
int play_board (Board* board, int x, int y, int piece);
int play_middle (Board* board, int player);
int utility (Board* board);
int get_length (Pos* list);
void sorted_plays (Pos* play_buffer, Board* board, int player);

void print_board (Board* board);

#endif
