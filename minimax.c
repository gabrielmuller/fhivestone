#include "minimax.h"

int play_minimax (Board* board, int player, int effort, int depth) {
    /*
     * Inicia o algoritmo de minimax.
     * Função paralelizada com OpenMP.
     * Retorna 1 se empate, senão 0.
     */


    // quantos tabuleiros foram analisados
    counter_t shared_counter = 0; 
    
    int minimizing = player == player2; // está minimizando?

    // melhor valor heurístico achado
    int best_value = 9999999;
    best_value *= minimizing ? 1 : -1;

    // posição com melhor valor heurístico
    int best_x = 0;
    int best_y = 0;

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
    int alpha = -100000000;
    int beta = 100000000;

    // qual valor alfa/beta será relevante
    int* ab = minimizing ? &beta : &alpha;

    // função que decide qual valor heurístico entre dois é o melhor
    int (*best)(int, int) = minimizing ? &min : &max;

    #pragma omp parallel
    {
    // tabuleiro para simulação
    Board* sim_board = copy_board(board);

    // memória para as demais profundidades usarem como fila de prioridade
    Pos* queue = malloc(sizeof(Pos)*(BOARD_SIZE*BOARD_SIZE+1)*(depth-2));

    #pragma omp for schedule(dynamic, 2)
    for (int i = 0; i < length; i++) {
        counter_t counter = 0;

        int x = shared_queue[i+1].x;
        int y = shared_queue[i+1].y;

        play_board(sim_board, x, y, player);

        int hval = minimax(sim_board,
                           queue,
                           effort,
                           depth-1,
                           alpha,
                           beta,
                           !minimizing,
                           &counter
                           );

        #pragma omp critical
        {
            int new_value = (*best)(best_value, hval);
            if (new_value != best_value) {
                best_x = x;
                best_y = y;
                best_value = new_value;
            }
            *ab = (*best)(*ab, best_value);
            progress++;
            printf("\r%d%%", (progress*100)/length);
            shared_counter += counter;
        } // end critical

        play_board(sim_board, x, y, empty);
    }

    free(queue);
    free_board(sim_board);

    } // end parallel
    printf("\r\n\n\n");
    printf("Caminhos analisados: %llu\n", shared_counter);

    free(shared_queue);
        
    play_board(board, best_x, best_y, player);
    return 0;
}

int minimax (Board* board,
             Pos* queue,
             int effort,
             int depth,
             int alpha,
             int beta,
             int minimizing,
             counter_t* counter
             ) {

    if (!depth || utility(board) > 0) {
        // se profundidade chegou ao limite ou alguém ganhou na simulação
        (*counter)++;
        return board->hval;
    }


    int best_value;
    int player = minimizing ? player2 : player1;
    int (*best)(int, int) = minimizing ? &min : &max;
    int* ab = minimizing ? &beta : &alpha;
    best_value = 900000;
    best_value *= minimizing ? 1 : -1;

    if (depth < 2) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            for (int y = 0; y < BOARD_SIZE; y++) {
                if (play_board(board, x, y, player) == invalid) {
                    continue;
                }

                best_value = (*best)(best_value, minimax(
                    board,
                    queue + BOARD_SIZE*BOARD_SIZE,
                    effort,
                    depth-1,
                    alpha,
                    beta,
                    !minimizing,
                    counter
                    )
                );

                play_board(board, x, y, empty);

                *ab = (*best)(*ab, best_value);
                if (alpha >= beta) return best_value;
            }
        }
        return best_value;
    }
                
    sorted_plays(queue, board, player);
    int length = min(get_length(queue), effort);

    for (int i = 0; i < length; i++) {
        int x = queue[i+1].x;
        int y = queue[i+1].y;

        play_board(board, x, y, player);
        best_value = (*best)(best_value, minimax(
            board,
            queue + BOARD_SIZE*BOARD_SIZE,
            effort,
            depth-1,
            alpha,
            beta,
            !minimizing,
            counter)
            );
        play_board(board, x, y, empty);

        *ab = (*best)(*ab, best_value);
        if (alpha >= beta) return best_value;
    }
    return best_value;
}
