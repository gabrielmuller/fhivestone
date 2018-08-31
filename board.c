/*
 * Tabuleiro
 *
 * Mantém uma cópia para cada orientação
 */
#include "board.h"

struct Board {
    row_t* horizontal;// tabuleiro normal
    row_t* vertical;  // tabuleiro transposto
    row_t* descending;// diagonal cima-esquerda para baixo-direita
    row_t* ascending; // diagonal baixo-esquerda para cima-direita
    int size;
};

Board* create_board (int size) {
    Board* board = malloc(sizeof(Board));
    board->horizontal = calloc(size, sizeof(row_t));
    board->vertical   = calloc(size, sizeof(row_t));
    board->descending = calloc(size*2 - 1, sizeof(row_t));
    board->ascending  = calloc(size*2 - 1, sizeof(row_t));
    board->size = size;
    return board;
}

void play_row (row_t* row, int pos, int piece) {
    *row |= piece << (2 * pos);
}

void play_orientation (row_t* row, int x, int y, int piece) {
    play_row(&row[y], x, piece);
}
    

void play_board (Board* board, int x, int y, int piece) {
    play_orientation(board->horizontal, x, y,                  piece);
    play_orientation(board->vertical,   y, x,                  piece);
    play_orientation(board->descending, x, x+y,                piece);
    play_orientation(board->ascending,  y, -x+y+board->size-1, piece);
}

#ifdef DISPLAY

#include <stdio.h>
void print_board (Board* board) {
    for (int i = 0; i < board->size; i++) {
        for (int j = 0; j < board->size; j++) {
            int piece = (board->horizontal[i] >> (j*2)) & 0x3;
            switch (piece) {
                case player1:
                    printf("O");
                    break;
                case player2:
                    printf("X");
                    break;
                case empty:
                    printf("_");
                    break;
                case offboard:
                    printf("E");
                    break;
                default:
                    printf("%X", piece);
            }
            printf(" ");
        }
        printf("\n");
    }
}
#endif
