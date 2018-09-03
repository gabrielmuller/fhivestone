
#include "minimax.h"

void cpu_vs_cpu () {
    Board* board = create_board();
    while (!utility(board)) {
        play_minimax(board, player1);
        play_minimax(board, player2);
        print_board(board);
    }
}

void player_vs_cpu() {
    Board* board = create_board();
    char* input = malloc(sizeof(char) * 3);
    for(;;) {
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
}

int main (void) {
    htable = generate_table(WIN_SIZE, VALUES);

    player_vs_cpu();
    //cpu_vs_cpu();

    return 0;
}

