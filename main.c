
#include "board.h"

int main (void) {
    int values[] = {1, 10, 100, 1000, 1000000};
    int* table = generate_table(5, values);

    Board* board = create_board(15);
    play_board(board, 0, 0, 0x1);
    play_board(board, 0, 5, 0x2);
    play_board(board, 3, 7, 0x1);

    print_board(board);

    return 0;
}
