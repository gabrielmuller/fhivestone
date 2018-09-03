#include "minimax.h"
#define STATS

#ifdef STATS
int counter;
#endif

int play_minimax (Board* board, int player) {
    
#ifdef STATS
    counter = 0;
#endif

    Board* sim_board = copy_board(board);
    int depth = DEPTH;
    int alpha = -10000000;
    int beta = 10000000;
    int value;
    int minimizing = player == player2;
    int (*best)(int, int) = minimizing ? &min : &max;
    int* ab = minimizing ? &beta : &alpha;
    value = 9999999;
    value *= minimizing ? 1 : -1;
    int best_x = 0;
    int best_y = 0;
    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {
            // se peça já foi jogada aqui ignora
            if (play_board(sim_board, x, y, player)) {
                continue;
            }
            int new_value = (*best)(value,
                    minimax(sim_board, depth-1, alpha, beta, !minimizing)
            );
            // apenas a camada mais superficial do minimax precisa
            // retornar uma posição
            if (new_value != value) {
                best_x = x;
                best_y = y;
                value = new_value;
            }
            *ab = (*best)(*ab, value);
            play_board(sim_board, x, y, empty);
            printf(".");
            fflush(stdout);
            if (alpha >= beta) goto end;
        }
    }

#ifdef STATS
    printf("Tabuleiros analisados: %d\n", counter);
    printf("Melhor valor previsto: %d\n", value);
#endif

    end:
    play_board(board, best_x, best_y, player);
    return value;
}

int minimax (Board* board, int depth, int alpha, int beta, int minimizing) {
    if (!depth) {
        return board->hval;
    }

#ifdef STATS
    counter++;
#endif

    int value;
    int player = minimizing ? player2 : player1;
    int (*best)(int, int) = minimizing ? &min : &max;
    int* ab = minimizing ? &beta : &alpha;
    value = 9999999;
    value *= minimizing ? 1 : -1;
    for (int x = 0; x < 7; x++) {
        for (int y = 0; y < 7; y++) {
            if (play_board(board, x, y, player)){
                continue;
            }
            value = (*best)(value, minimax(board, depth-1, alpha, beta, !minimizing));
            *ab = (*best)(*ab, value);
            play_board(board, x, y, empty);
            if (alpha >= beta) return value;
        }
    }
    return value;
}





