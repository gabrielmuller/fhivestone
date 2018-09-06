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
    int last_x = board->last_x;
    int last_y = board->last_y;

    // Cria fila ordenada de posições a testar
    Pos* queue = malloc(sizeof(Pos) * (BOARD_SIZE*BOARD_SIZE+1));
    sorted_plays(queue, sim_board, player);
    int length = get_length(queue);
    queue++; // primeiro elemento é length, desconsidera

    //for (int i = 0; i < length; i++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        /*
        int x = queue[i].x;
        int y = queue[i].y;
        */

        if (play_board(sim_board, x, y, player) == invalid) {
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
        play_board(sim_board, x, y, empty);
        //printf("\r%d", (i*100)/length);
        fflush(stdout);
        *ab = (*best)(*ab, value);
        if (alpha >= beta) break;
    }}

#ifdef STATS
    printf("\n");
    printf("Tabuleiros analisados: %d\n", counter);
    printf("Melhor valor previsto: %d\n", value);
#endif

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

    /*
    if (depth < 2) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                value = (*best)(value, minimax(board, depth-1, alpha, beta, !minimizing));
                if (play_board(board, x, y, empty)) {
                    continue;
                }

                *ab = (*best)(*ab, value);
                if (alpha >= beta) return value;
            }
        }
        return value;
    }
                
    // Cria fila ordenada de posições a testar
    Pos* queue = sorted_plays(board, player);
    int length = get_length(queue);
    queue++; // primeiro elemento é length, desconsidera

    for (int i = 0; i < length; i++) {
        int x = queue[i].x;
        int y = queue[i].y;

        value = (*best)(value, minimax(board, depth-1, alpha, beta, !minimizing));
        play_board(board, x, y, empty);

        *ab = (*best)(*ab, value);
        if (alpha >= beta) return value;
    }*/
    for (int x = 0; x < BOARD_SIZE; x++) {
        for (int y = 0; y < BOARD_SIZE; y++) {
            if (play_board(board, x, y, player) == invalid) {
                continue;
            }

            value = (*best)(value, minimax(board, depth-1, alpha, beta, !minimizing));
            play_board(board, x, y, empty);

            *ab = (*best)(*ab, value);
            if (alpha >= beta) return value;
        }
    }
    return value;
}
