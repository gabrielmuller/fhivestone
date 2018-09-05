
#include "minimax.h"

void cpu_vs_cpu () {
    Board* board = create_board();
    play_board(board, 7, 7, player2);
    for (int is_p1 = 1; !utility(board); is_p1 = !is_p1) {
        int player = is_p1 ? player1 : player2;
        play_minimax(board, player);
        print_board(board);
    }
}

void player_vs_cpu() {
    Board* board = create_board();
    char* input = malloc(sizeof(char) * 3);
    int who_won;
    while (who_won = !utility(board)) {
        print_board(board);
        printf("Vez do humano\n");
        fflush(stdout);
        size_t size = 3;
        getline(&input, &size, stdin);
        char cy[] = {input[0], '\0'};
        char cx[] = {input[1], '\0'};
        int y = strtol(cy, NULL, BOARD_SIZE);
        int x = strtol(cx, NULL, BOARD_SIZE);
        if (play_board(board, x, y, player1)) {
            printf("Você não pode jogar aí!\n");
            fflush(stdout);
        } else {
            print_board(board);
            printf("CPU pensando");
            play_minimax(board, player2);
        }
    }
    printf("----------\n \
            %s ganhou!\n \
            ----------\n",
            who_won == player1 ? "Você" : "CPU");
}

int main (void) {
    htable = generate_table(WIN_SIZE, VALUES);

#ifdef BENCHMARK
    cpu_vs_cpu();
    return 0;
#endif
    //player_vs_cpu();
    Board* board = create_board();
    play_board(board, 7, 7, player1);
    play_board(board, 7, 8, player2);
    Pos* pos = sorted_plays(board, player1);

    return 0;
}

