#include "node.h"
#include <stdlib.h>

Node* create_node(Position pos) {
    Node* node = malloc(sizeof(Node));
    node->pos = pos;
    return node;
}
