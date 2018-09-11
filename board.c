/*
 * Tabuleiro
 *
 * Mantém uma cópia para cada orientação
 */
#include "board.h"

int* htable;

Board* create_board () {
    /*
     * Aloca memória e inicializa um tabuleiro novo.
     */

    Board* board = malloc(sizeof(Board));

    board->horizontal = calloc(BOARD_SIZE, sizeof(row_t));
    board->vertical   = calloc(BOARD_SIZE, sizeof(row_t));

    // quantidade de fileiras diagonais
    board->diag_rows = BOARD_SIZE * 2 - 1; 

    board->descending = calloc(board->diag_rows, sizeof(row_t));
    board->ascending  = calloc(board->diag_rows, sizeof(row_t));
    
    // sinalizar até onde vai cada fileira diagonal

    //row_size faz 1, 2, 3, ... 14, 15, 14, 13, 12... 2, 1
    int row_size = 1;

    int inc = 1;
    for (int i = 0; i < board->diag_rows; i++) {
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

void free_board (Board* board) {
    /*
     * Libera tabuleiro da memória.
     */

    free(board->horizontal);
    free(board->vertical);
    free(board->descending);
    free(board->ascending);
    free(board);
}


Board* copy_board (Board* other) {
    /*
     * Cria uma cópia do tabuleiro *other
     */

    Board* board = malloc(sizeof(Board));
    *board = *other;

    board->horizontal = malloc(BOARD_SIZE * sizeof(row_t));
    board->vertical = malloc(BOARD_SIZE * sizeof(row_t));
    board->ascending = malloc(board->diag_rows * sizeof(row_t));
    board->descending = malloc(board->diag_rows * sizeof(row_t));

    memcpy(board->horizontal,
           other->horizontal,
           BOARD_SIZE * sizeof(row_t)
           );
    memcpy(board->vertical,
           other->vertical,
           BOARD_SIZE * sizeof(row_t)
           );
    memcpy(board->ascending,
           other->ascending,
           board->diag_rows * sizeof(row_t)
           );
    memcpy(board->descending,
           other->descending,
           board->diag_rows * sizeof(row_t)
           );

    return board;
}

int select_affected(row_t row, int pos) {
    /*
     * Seleciona os espaços da fileira row que têm valor heurístico
     * afetado por uma jogada em pos.
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
    // se for 00, apaga
    if (piece) {
        *row |= piece << 2 * pos; 
    } else {
        *row &= ~(0x3 << 2 * pos);
    }
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

int play_board (Board* board, int x, int y, int piece) {
    /*
     * Joga uma peça no tabuleiro.
     * Se peça for empty, remove peça
     * x, y é a posição relativa ao tabuleiro (orientação horizontal)
     * retorna 1 se não é possível jogar em x, y.
     * retorna 2 se jogo já acabou.
     */
    if (((board->horizontal[y] >> x * 2) & 0x3) && piece) {
        return invalid;
    }
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

    return utility(board);
}

int play_middle (Board* board, int player) {
    /*
     * Função de utilidade, faz uma jogada no centro do tabuleiro.
     */

    play_board(board, BOARD_SIZE/2, BOARD_SIZE/2, player);
}

int utility (Board* board) {
    /*
     * Retorna jogador que ganhou, ou zero se ninguém ganhou ainda.
     */
    if (board->hval > WIN) {
        return player1;
    } else if (board->hval < -WIN) {
        return player2;
    }
    return empty;
}

int compare_dec (const void* a, const void* b) {
    /*
     * Compara jogadas em ordem decrescente.
     * Usado com o qsort.
     */
    return ((Pos*)b)->hval - ((Pos*)a)->hval;
}

int compare_inc (const void* a, const void* b) {
    /*
     * Compara jogadas em ordem crescente.
     * Usado com o qsort.
     */
    return ((Pos*)a)->hval - ((Pos*)b)->hval;
}

int get_length (Pos* list) {
    /*
     * Tamanho da lista é armazenado no primeiro elemento.
     */
    return list[0].hval;
}

void sorted_plays (Pos* play_buffer, Board* board, int player) {
    /*
     * Retorna array com todas as jogadas em ordem decrescente de valor
     * heurístico
     *
     * Primeiro elemento do retorno armazena o tamanho real do array
     */

    int length = 0;
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (play_board(board, x, y, player) == invalid) { 
                // Jogada inválida, não coloca no array
                continue;
            }
            length++;
            play_buffer[length].x = x;
            play_buffer[length].y = y;
            play_buffer[length].hval = board->hval;

            // Restaura tabuleiro
            play_board(board, x, y, empty);
        } 
    } 
    // Ordena lista
    play_buffer[0].hval = length;
    if (player == player1) {
        qsort(play_buffer+1, length, sizeof(Pos), compare_dec);
    } else {
        qsort(play_buffer+1, length, sizeof(Pos), compare_inc);
    }
}

void print_board (Board* board) {
    /*
     * Mostra o tabuleiro no stdout.
     */
    printf("Valor heurístico: %d\n", board->hval);
    printf("  ");

    // printa fileira 0 1 2 3 4 5 ...
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf(" %X", i);
    }

    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%X ", i);
        int next_is_space = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            int piece = (board->horizontal[i] >> (j*2)) & 0x3;
            switch (piece) {
                case player1:
                    printf(" ○");
                    next_is_space = 1;
                    break;
                case player2:
                    printf(" ●");
                    next_is_space = 1;
                    break;
                case empty:
                    if (next_is_space) {
                        printf(" ┼");
                        next_is_space = 0;
                    } else {
                        printf("╌┼");
                    }
                    break;
                case offboard:
                    printf(" E");
                    break;
                default:
                    printf("%X", piece);
            }
        }
        printf("\n");
    }
}
