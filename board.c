/*
 * Tabuleiro
 *
 * Mantém uma cópia para cada orientação
 */
#include "board.h"
#include <stdio.h>

int* htable;

struct Board {
    row_t* horizontal; // tabuleiro normal
    row_t* vertical;   // tabuleiro transposto
    row_t* descending; // diagonal cima-esquerda para baixo-direita
    row_t* ascending;  // diagonal baixo-esquerda para cima-direita
    int hval;          // valor heurístico
    int size;          // dimensões
};

Board* create_board () {
    Board* board = malloc(sizeof(Board));

    board->horizontal = calloc(BOARD_SIZE, sizeof(row_t));
    board->vertical   = calloc(BOARD_SIZE, sizeof(row_t));

    int diag_rows = BOARD_SIZE * 2 - 1; // quantidade de fileiras diagonais
    board->descending = calloc(diag_rows, sizeof(row_t));
    board->ascending  = calloc(diag_rows, sizeof(row_t));
    
    // sinalizar até onde vai cada fileira diagonal

    //row_size faz 1, 2, 3, ... 14, 15, 14, 13, 12... 2, 1
    int row_size = 1;

    int inc = 1;
    for (int i = 0; i < diag_rows; i++) {
        for (int j = 0; j < BOARD_SIZE - row_size; j++) {
            board->ascending[i]  |= (0x3 << (BOARD_SIZE-1) * 2) >> j * 2;
            board->descending[i] |= (0x3 << (BOARD_SIZE-1) * 2) >> j * 2;
        }
        row_size += inc;
        if (row_size == BOARD_SIZE) {
            inc = -1;
        }
    }

    board->hval = 0;

    return board;
}

int select_affected(row_t row, int pos) {
    /*
     * Seleciona as peças da fileira que são
     * afetadas por uma jogada em pos.
     */
    int start = pos - WIN_SIZE + 1;
    int end = pos + WIN_SIZE;
    start = start < 0 ? 0 : start;
    end = end > BOARD_SIZE ? BOARD_SIZE : end;
    row >>= start * 2;
    row |= BITMASKS[end - start];
    row &= 0x3ffff;
    return row;
}

int play_row (row_t* row, int pos, int piece) {
    /*
     * Joga uma peça numa fileira específica.
     * pos é a posição relativa à fileira.
     * retorna variação no valor heurístico.
     */
    int prev_hval = htable[select_affected(*row, pos)];
    // adiciona peça
    // obs: se estiver fora do tabuleiro (11), continua fora (11)
    *row |= piece << (2 * pos); 
    return htable[select_affected(*row, pos)] - prev_hval;
}

int play_orientation (row_t* row, int x, int y, int piece) {
    /*
     * Joga uma peça numa das cópias do tabuleiro.
     * x, y é a posição relativa à orientação.
     * retorna variação no valor heurístico.
     */
    return play_row(&row[y], x, piece);
}
    
int min (int x, int y) {
    return x < y ? x : y;
}

void play_board (Board* board, int x, int y, int piece) {
    /*
     * Joga uma peça no tabuleiro.
     * x, y é a posição relativa ao tabuleiro (orientação horizontal)
     */
    board->hval += play_orientation(board->horizontal, x, y,   piece);
    board->hval += play_orientation(board->vertical,   y, x,   piece);
    board->hval += play_orientation(
            board->descending,
            min(BOARD_SIZE-1-x, y),
            x+y,
            piece
    );
    board->hval += play_orientation(
            board->ascending,
            min(x, y), 
            BOARD_SIZE-1-x+y,
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

void print_board (Board* board) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
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
