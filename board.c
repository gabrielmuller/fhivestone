/*
 * Tabuleiro
 *
 * Mantém uma cópia para cada orientação
 */
#include "board.h"

int* htable;

struct Board {
    row_t* horizontal; // tabuleiro normal
    row_t* vertical;   // tabuleiro transposto
    row_t* descending; // diagonal cima-esquerda para baixo-direita
    row_t* ascending;  // diagonal baixo-esquerda para cima-direita
    int hval;          // valor heurístico
    int size;          // dimensões
};

Board* create_board (int size) {
    Board* board = malloc(sizeof(Board));

    board->horizontal = calloc(size, sizeof(row_t));
    board->vertical   = calloc(size, sizeof(row_t));
    board->descending = calloc((size - WIN_SIZE)*2 + 1, sizeof(row_t));
    board->ascending  = calloc((size - WIN_SIZE)*2 + 1, sizeof(row_t));
    board->size = size;
    board->hval = 0;

    return board;
}

int play_row (row_t* row, int pos, int piece) {
    /*
     * Joga uma peça numa fileira específica.
     * pos é a posição relativa à fileira.
     * retorna variação no valor heurístico.
     */
    int shift = pos - WIN_SIZE + 1;
    int bitmask_size = WIN_SIZE * 2 - 1;

    if (shift < 0) {
        bitmask_size += shift;
        shift = 0;
    }

    int bitmask = BITMASKS[bitmask_size];
    int prev_hval = htable[(*row >> shift) & bitmask];
    *row |= piece << (2 * pos);
    return htable[(*row >> shift) & bitmask] - prev_hval;
}

int play_orientation (row_t* row, int x, int y, int piece) {
    /*
     * Joga uma peça numa das cópias do tabuleiro.
     * x, y é a posição relativa à orientação.
     * retorna variação no valor heurístico.
     */
    return play_row(&row[y], x, piece);
}
    

void play_board (Board* board, int x, int y, int piece) {
    /*
     * Joga uma peça no tabuleiro.
     * x, y é a posição relativa ao tabuleiro (orientação horizontal)
     */
    board->hval += play_orientation(board->horizontal, x, y,   piece);
    board->hval += play_orientation(board->vertical,   y, x,   piece);
    board->hval += play_orientation(board->descending, x, x+y, piece);
    board->hval += play_orientation(board->ascending,  y, 
            -x+y+board->size-1,
            piece
    );
}

int utility (Board* board) {
    if (board->hval > WIN) {
        return player1;
    }
    else if (board->hval < -WIN) {
        return player2;
    }
    return empty;
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
    printf("hval: %d\n", board->hval);
}

#endif
