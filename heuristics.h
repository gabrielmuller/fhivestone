#include <stdio.h>
#include <stdlib.h>

#define INVALID 0xBABACA

enum vals {empty=0x00, player1=0x01, player2=0x02, invalid=0x03};
static int* bits_to_array (int bits, int length);
static int evaluate_sequence (int* sequence, int length, int* values);
int* generate_table (int seq_length, int* values);
int evaluate_matrix (
        int* board,
        int row_n,
        int col_n,
        int seq_length, 
        int* table
    );
