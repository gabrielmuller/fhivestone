#include <stdlib.h>
#include "constants.h"

static int* bits_to_array (row_t bits, int length);
static int evaluate_sequence (int* sequence, int length, int* values);
int* generate_table (int seq_length, int* values);
int evaluate_matrix (
        int* board,
        int row_n,
        int col_n,
        int seq_length, 
        int* table
    );
