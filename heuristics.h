#include <stdio.h>
#include <stdlib.h>

#define INVALID 0xBABACA

typedef int16_t seq; 
typedef int32_t row;

enum vals {empty=0x0, player1=0x1, player2=0x2, invalid=0x3};
static int* seq_to_array (seq bits, int length);
static int evaluate_sequence (int* sequence, int length, int* values);
int* generate_table (int seq_length, int* values);
