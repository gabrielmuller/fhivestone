#include "board.h"

// Espaços a considerar para jogadas futuras
bool* spaces;

// Tabuleiro para simular jogadas
Board* board;

void add_spaces (int x, int y) {
    /*
     * Adiciona espaços a considerar ao redor de (x, y)
     */
    for (int i = x - SPACE_RADIUS; i < x + SPACE_RADIUS; i++) {
        for (int j = y - SPACE_RADIUS; j < y + SPACE_RADIUS; j++) {
            spaces[i][j] = true;
        }
    }
void start_minimax (Board* board) {
    spaces = calloc(BOARD_SIZE * BOARD_SIZE, sizeof(bool));
    Board sim_board = *board; // copia tabuleiro


