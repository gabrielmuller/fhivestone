#include "minimax.h"

int play_minimax (Board* board, int player, int effort, int depth) {
    /*
     * Inicia o algoritmo de minimax.
     * Função paralelizada com OpenMP.
     * Retorna 1 se empate, senão 0.
     */


    // quantos tabuleiros foram analisados
    unsigned long long int shared_counter = 0; 
    
    int minimizing = player == player2; // está minimizando?

    // melhor valor heurístico achado
    int value = 9999999;
    value *= minimizing ? 1 : -1;

    // posição com melhor valor heurístico
    int best_x = -1;
    int best_y = -1;

    // usado para porcentagem
    int progress = 0;

    // Cria fila ordenada de posições a testar
    Pos* shared_queue = malloc(sizeof(Pos) * (BOARD_SIZE*BOARD_SIZE+1));
    sorted_plays(shared_queue, board, player);

    // analisa o menor entre esforço e número de espaços possíveis.
    int length = min(get_length(shared_queue), effort);
    
    if (!length) {
        // não sobrou nenhum espaço pra jogar
        return 1;
    }

    #pragma omp parallel
    {
    // tabuleiro para simulação
    Board* sim_board = copy_board(board);
    Pos* queue = malloc(sizeof(Pos)*(BOARD_SIZE*BOARD_SIZE+1)*(depth-1));

    int alpha = -100000000;
    int beta = 100000000;

    // função que decide qual valor heurístico entre dois é o melhor
    int (*best)(int, int) = minimizing ? &min : &max;

    // qual valor alfa/beta será relevante
    int* ab = minimizing ? &beta : &alpha;

    #pragma omp for schedule(dynamic, 1)
    for (int i = 0; i < length; i++) {
        unsigned long long counter = 0;
        printf("\r%d%%", (progress*100)/length);

        int x = shared_queue[i+1].x;
        int y = shared_queue[i+1].y;

        play_board(sim_board, x, y, player);

        int new_value = (*best)(value,
                minimax(sim_board, queue, effort,
                        depth-1, alpha, beta, !minimizing, &counter)
        );

        #pragma omp critical
        {
            if (new_value != value) {
                best_x = x;
                best_y = y;
                value = new_value;
            }
            progress++;
            shared_counter += counter;
        } // end critical

        play_board(sim_board, x, y, empty);
        *ab = (*best)(*ab, value);
    }
    free(queue);
    free_board(sim_board);

    } // end parallel

    printf("\r\n\n\n");
    printf("Caminhos analisados: %d\n", shared_counter);

    free(shared_queue);
        
    play_board(board, best_x, best_y, player);
    return 0;
}

int minimax (Board* board, Pos* queue, int effort, int depth, int alpha, int beta, int minimizing, unsigned long long int* counter) {

    if (!depth || utility(board) > 0) {
        (*counter)++;
        return board->hval;
    }


    int value;
    int player = minimizing ? player2 : player1;
    int (*best)(int, int) = minimizing ? &min : &max;
    int* ab = minimizing ? &beta : &alpha;
    value = 900000;
    value *= minimizing ? 1 : -1;

    if (depth < 2) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                if (play_board(board, x, y, player) == invalid) {
                    continue;
                }

                value = (*best)(value, minimax(board, queue + BOARD_SIZE*BOARD_SIZE, effort, depth-1, alpha, beta, !minimizing, counter));
                play_board(board, x, y, empty);

                *ab = (*best)(*ab, value);
                if (alpha >= beta) return value;
            }
        }
        return value;
    }
                
    sorted_plays(queue, board, player);
    int length = min(get_length(queue), effort);

    for (int i = 0; i < length; i++) {
        int x = queue[i+1].x;
        int y = queue[i+1].y;

        play_board(board, x, y, player);
        value = (*best)(value, minimax(board, queue + BOARD_SIZE*BOARD_SIZE, effort, depth-1, alpha, beta, !minimizing, counter));
        play_board(board, x, y, empty);

        *ab = (*best)(*ab, value);
        if (alpha >= beta) return value;
    }
    return value;
}
