#include "heuristics.h"

int main (void) {
    int values[] = {1, 10, 100, 1000, 1000000};
    int* table = generate_table(5, values);
    printf("%d, %d, %d\n", table[0x117], table[0x212], table[0x115]);

    return 0;
}
