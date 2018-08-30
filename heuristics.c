#include "heuristics.h"

/*
 *  Define função de heurística.
 *
 *  Como cada espaço no tabuleiro tem 3 valores possíveis e cada sequência
 *  tem 5 espaços, há apenas 3^5 = 243 possíveis valores.
 *  Para facilitar, usa-se a seguinte representação para os espaços:
 *
 *  00: vazio            : _
 *  01: peça do jogador 1: O
 *  10: peça do jogador 2: X
 *  11: fora do tabuleiro
 *
 *  Uma sequência de 5 espaços pode ser codificada em um inteiro, que é usado
 *  para consultar a tabela de heurística.
 *  Na prática, há 2^10 = 1024 possíveis valores.
 *
 *  Com a tabela montada, consultar o valor heurístico de uma sequência
 *  é feito em tempo constante.
 */

static int* bits_to_array (row bits, int length) {
    int* array = malloc(sizeof(int) * length);

    for (int i = 0; i < length; i++) {
        array[i] = (bits >> (2 * i)) & 0x03;
    }

    return array;
}

static int evaluate_sequence (int* sequence, int length, int* values) {
    /*
     * Calcula valor de uma sequência de 5.
     * Usado para montar a tabela.
     */

    int p1_count = 0;
    int p2_count = 0;

    // Conta quantas peças de cada jogador
    for (int i = 0; i < length; i++) {
        switch (sequence[i]) {
            case player1:
                p1_count++;
                break;
            case player2:
                p2_count++;
                break;
        }
    }

    if ((p1_count && p2_count) || (!p1_count && !p2_count)) {
        //peça de ambos jogadores   nenhuma peça
        return 0;

    } else if (p1_count) {
        // P1 em vantagem
        return values[p1_count - 1];

    } else if (p2_count) {
        // P2 em vantagem
        return -values[p2_count - 1];
    }
}

int* generate_table (int small_length, int* values) {
    /*
     *  small_length: número de espaços (ex. 5)
     *  values: array de tamanho [small_length] que define o valor de
     *  cada caso (1 peça, 2 peças, 3 peças...)
     *  Retorna tabela de heurística em um array de inteiros. 
     *
     *  A otimização dessa função não é muito importante, já que só será
     *  chamada uma vez e demora ~10ms.
     */

     // tamanho da fileira que uma jogada afeta em uma orientação (9)
     int big_length = small_length * 2 - 1;

     int table_length = 1 << (big_length * 2); // 2^(big_length*2)
     int* table = calloc(table_length, sizeof(int));

     for (row big = 0; big < table_length; big++) {
        for (int small_offset = 0; small_offset < small_length; small_offset++) {
            int small = (big >> 2 * small_offset) & 0x3FF; // seleciona 5 espaços (10 bits) dos 9
            int* pieces = bits_to_array(small, small_length);
            table[big] += evaluate_sequence(pieces, small_length, values);
        }
     }

     return table;
}
