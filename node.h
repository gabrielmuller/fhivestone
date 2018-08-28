typedef struct Node Node;
typedef struct Heap Heap;

typedef struct {
    short int x;
    short int y;
} Position;

struct Heap {
    Heap* left;
    Heap* right;


struct Node {
    int value; // Valor heurístico
    Position pos; // Onde a jogada foi feita
    Heap* children;
};

Node* create_node (Position pos);
