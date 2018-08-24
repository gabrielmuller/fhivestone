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
 *  11: não usado
 *
 *  Uma sequência de 5 espaços pode ser codificada em um inteiro, que é usado
 *  para consultar a tabela de heurística.
 *  Na prática, há 2^10 = 1024 possíveis valores.
 *
 *  Com a tabela montada, consultar o valor heurístico de uma sequência
 *  é feito em tempo constante.
 */

static int* seq_to_array (seq bits, int length) {
    /*
     *  Dada uma sequência codificada em bits, retorna um array de inteiros,
     *  onde cada inteiro é uma peça.
     */
    int* array = malloc(sizeof(int) * length);

    for (int i = 0; i < length; i++) {
        array[i] = (bits >> (2 * i)) & 0x03;
    }

    return array;
}

static int evaluate_sequence (int* sequence, int length, int* values) {
    /*
     * Calcula valor de uma sequência.
     * Usado para montar a tabela.
     */

    int p1_count = 0;
    int p2_count = 0;

    // Conta quantas peças de cada jogador
    for (int i = 0; i < length; i++) {
        switch (sequence[i]) {
            case invalid:
                return INVALID;
            case player1:
                p1_count++;
                break;
            case player2:
                p2_count++;
                break;
        }
    }

    if (p1_count && p2_count) {
        // Há peças de ambos jogadores: ninguém pode ganhar
        return 0;

    } else if (p1_count) {
        // P1 em vantagem
        return values[p1_count - 1];

    } else if (p2_count) {
        // P2 em vantagem
        return -values[p2_count - 1];
    } else {
        // Não há peças
        return 0;

    }
}

int* generate_table (int seq_length, int* values) {
    /*
     *  seq_length: número de espaços (ex. 5)
     *  values: array de tamanho [seq_length] que define o valor de
     *  cada caso (1 peça, 2 peças, 3 peças...)
     *  Retorna tabela de heurística em um array de inteiros. 
     *
     *  A otimização dessa função não é muito importante, já que só será
     *  chamada uma vez e demora 1ms.
     */

     int table_length = 1 << (seq_length * 2); // 2^(seq_length*2 bits)
     int* table = malloc(sizeof(seq) * table_length);

     int space[seq_length];
            
     for (seq i = 0; i < table_length; i++) {
        int* pieces = seq_to_array(i, seq_length);
        table[i] = evaluate_sequence(pieces, seq_length, values);
     }

     return table;
}
