
#include "minimax.h"

void cpu_vs_cpu (int efforts[2], int depths[2]) {
    /*
     * Joga CPU contra CPU.
     * Útil para testar IA e fazer benchmarks.
     */

    Board* board = create_board();
    play_middle(board, player2);
    int players[] = {player1, player2};

    for (int turn = 0; !utility(board); turn = !turn) {
        int player = players[turn];
        int effort = efforts[turn];
        int depth  =  depths[turn];

        if (play_minimax(board, player, effort, depth)) {
            printf("Empate\n");
            break;
        }
        effort += EFFORT_INC;

        print_board(board);
    }

    free_board(board);
}

void player_vs_cpu (int cpu_starts, int effort, int depth) {
    /*
     * Joga CPU contra humano.
     * cpu_starts é 1 se CPU começa.
     */

    Board* board = create_board();

    // buffer para receber jogadas pelo stdin
    size_t size = 3;
    char* input = malloc(sizeof(char) * size);

    // quando alguém ganha, armazena o ganhador
    int who_won = 0;

    if (cpu_starts) {
        // CPU sempre começa jogando no meio
        play_middle(board, player2);
    }

    while(!who_won) {
        print_board(board);
        printf("\nSua vez! [linha][coluna]\n");

        // lê jogada do stdin
        getline(&input, &size, stdin);
        char cy[] = {input[0], '\0'};
        char cx[] = {input[1], '\0'};
        int y = strtol(cy, NULL, BOARD_SIZE);
        int x = strtol(cx, NULL, BOARD_SIZE);

        if (x < 0 | y < 0 | x >= BOARD_SIZE | y >= BOARD_SIZE) {
            printf("\n\aEssa posição está fora do tabuleiro!\n");
        } else switch (play_board(board, x, y, player1)) {
            case invalid:
                printf("\n\aJá tem uma peça aí!\n");
                break;
            case player1:
                who_won = player1;
                continue;
            case 0:
                print_board(board);
                printf("CPU pensando 🤔\n");
                if (play_minimax(board, player2, effort, depth)) {
                    who_won = invalid;
                }
                if (utility(board) == player2) {
                    who_won = player2;
                }
        }
        effort += EFFORT_INC * 2;
    }

    print_board(board);
    if (who_won == invalid) {
        printf("Empate!\n");
    }

    printf("\n\a%s ganhou!\n\n", who_won == player1 ? "😄 Você" : "🤖 CPU");

    free(input);
    free_board(board);
}

int main (int argc, char** argv) {
    /*
     * Ponto de entrada do programa
     */

    // valores padrão
    int depth = DEFAULT_DEPTH;
    int effort = DEFAULT_EFFORT;
    int autoplay = 0;
    int cpu_starts = 0;
    int efforts[] = {DEFAULT_EFFORT, DEFAULT_EFFORT};
    int depths[] = {DEFAULT_DEPTH, DEFAULT_DEPTH};

    // analisa argumentos de entrada
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--depth")) {
            depth = atoi(argv[i+1]);
        } else if (!strcmp(argv[i], "--effort")) {
            effort = atoi(argv[i+1]);
        } else if (!strcmp(argv[i], "--complete")) {
            effort = 999;
            efforts[0] = 999;
            efforts[1] = 999;
        } else if (!strcmp(argv[i], "--cpu-starts")) {
            cpu_starts = 1;
        } else if (!strcmp(argv[i], "--autoplay")) {
            autoplay = 1;
        } else if (!strcmp(argv[i], "--efforts")) {
            efforts[0] = atoi(argv[i+1]);
            efforts[1] = atoi(argv[i+2]);
        } else if (!strcmp(argv[i], "--depths")) {
            depths[0] = atoi(argv[i+1]);
            depths[1] = atoi(argv[i+2]);
        }
    }

    // saída sem buffer para não precisar de flush
    setbuf(stdout, NULL);

    // pré-calcula valores heurísticos de sequências de 9
    htable = generate_table(WIN_SIZE, VALUES);


    if (autoplay) {
        for (int i = 0; i < 2; i++) {
            printf("CPU %d, profundidade %d, esforço %d\n", i+1, depths[i], efforts[i]);
        }
        cpu_vs_cpu(efforts, depths);
    } else {
        printf("Analisando %d jogadas para frente\n", depth);
        printf("Considerando %d melhores jogadas\n", effort);
        player_vs_cpu(cpu_starts, effort, depth);
    }

    free(htable);
    return 0;
}

