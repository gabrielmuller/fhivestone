# IA de Gomoku

## Instalação e execução

Para executar da forma mais simples, execute:
    make
    ./fhive

Outras opções do make:
    make debug  # flags de debug
    make single # sem multithreading

### Parâmetros de execução
``--autoplay``   CPU contra CPU
``--complete``   desativa otimização do *effort*.

Parâmetros de execução no modo humano vs CPU (padrão):
``--effort N``   analisa as N melhores jogadas. Explicado adiante.
``--depth N``    define nível de profundidade
``--cpu-starts`` deixa CPU fazer a primeira jogada

Parâmetros de execução no modo CPU vs CPU (``--autoplay``):
``--efforts N M``   parâmetro *effort* para CPU1 e CPU2
``--depths N M``    profundidade para CPU1 e CPU2

## Conceito da heurística

Conceitualmente, o valor heurístico de um tabuleiro é a soma dos valores
atribuídos a todas subsequências sobrepostas do tabuleiro. Sequências com
peças de tipos diferentes e sequências vazias não pontuam, enquanto sequências
com peças de apenas um tipo pontuam de acordo com a quantidade de peças.

    PEÇAS    VALOR
    1        1
    2        10
    3        100
    4        10000
    5        1000000

Por exemplo, considere a primeira jogada de uma partida no meio do tabuleiro.
Analisando uma fileira, temos as seguintes subsequências:

    _ _ _ _ O _ _ _ _

    ~ ~ ~ ~ ~           1
      ~ ~ ~ ~ ~         1
        ~ ~ ~ ~ ~       1
          ~ ~ ~ ~ ~     1
            ~ ~ ~ ~ ~   1

Com cinco subsequências pontuando 1 na horizontal, o jogador ganha 5 pontos.
Considerando as demais orientações, o total é 20.

### Utilidade

Definida em função da heurística. Se o valor absoluto é maior que 900000,
deve haver ao menos uma subsequência com 5 em seguida.

## Estruturas de dados

### Tabuleiro
O tabuleiro é armazenado com um inteiro de 32 bits por fileira, com cada espaço 
do tabuleiro representado por 2 bits:

    vazio:             00
    jogador 1:         01 
    jogador 2:         10
    fora do tabuleiro: 11

Exemplo de fileira:

       F  _  _  _  O  X  _  _  X  X  X  X  O  _  _  _  
       XX 00 00 00 01 10 00 00 10 10 10 10 01 00 00 00
    0x   0     0     6     0     A     A     4     0

    = 0x0060AA40

Também são mantidas quatro cópias do tabuleiro, uma para cada orientação. O
valor 11 (fora do tabuleiro) é relevante no caso das diagonais, nas quais cada
fileira tem um tamanho distinto.

### Tabela de heurística
No contexto de uma fileira, uma jogada muda o valor heurístico apenas das
subsequências a seu redor. Mais especificamente, apenas uma seção de no máximo
9 espaços da fileira é afetada.

Podemos, então, calcular o valor heurístico de todas as possíveis sequências
de 9 espaços. Como cada espaço ocupa 2 bits, precisamos de uma tabela com 2^18
entradas. A tabela em questão é um simples array de inteiros, e o índice é a
sequência de tamanho 9. Isso é implementado em ``heuristics.c:generate_table``.

### Fila de prioridade

Em todas camadas do minimax exceto as duas últimas, as possíveis jogadas são
colocadas em uma fila de prioridade de acordo com o valor heurístico. A busca
começa, então, pelas jogadas mais promissoras. Implementado em
``board.c:sorted_plays``.

Na prática, observou-se que, apesar de normalmente termos mais de 200 possíveis
jogadas, apenas as melhores 20 jogadas têm alguma chance de serem a melhor.
Usamos o parâmetro *effort* para definir o número base de jogadas a considerar.
Por padrão, esse valor começa em 25, e aumenta em 4 a cada jogada.

Essa otimização deixa a busca extremamente mais rápida, mas perde-se a garantia
que temos de fato a melhor jogada em uma profundidade específica. Para
desativar essa otimização, use o argumento ``--complete``.

## Algoritmos

### Jogada no tabuleiro

Uma jogada no tabuleiro atualiza as quatro cópias, uma para cada orientação.
Implementado em ``board.c:play_board``.

### Fila de prioridade

Cria um array com todas posições de jogadas (``struct Pos``) e ordena usando
a função ``qsort`` da biblioteca padrão.

### Minimax

O algoritmo minimax funciona de forma diferente em algumas camadas.

#### Primeira camada (mais superficial)

Única camada que, além de manter o melhor valor heurístico, guarda as
coordenadas da melhor jogada. Também é responsável por dividir o trabalho entre
as threads, usando escalonamento dinâmico do OpenMP.

#### Penúltima camada (profundidade 1)

Não faz uma fila de prioridade, já que a próxima chamada a minimax apenas
retornará o valor heurístico do tabuleiro. Simplesmente itera por todos
espaços do tabuleiro, ignorando os que já foram ocupados.

#### Última camada, ou camada com valor utilidade

Retorna o valor heurístico do tabuleiro.

#### Demais camadas

Funciona, de forma simples, da seguinte forma:

    fila = jogadas ordenadas por melhor valor heurístico
    para os primeiros effort elementos da fila:
        jogue no tabuleiro de simulação
        melhor valor = melhor entre (melhor valor, minimax(...))
        atualiza alfa/beta
        desfaça a jogada
    retorne melhor valor
