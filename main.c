
#include "board.h"

int main (void) {
    htable = generate_table(WIN_SIZE, VALUES);

    Board* board = create_board(BOARD_SIZE);
    play_board(board, 0, 0, 0x1);
    print_board(board);
    play_board(board, 0, 5, 0x2);
    print_board(board);
    play_board(board, 3, 7, 0x1);
    print_board(board);

    return 0;
}
