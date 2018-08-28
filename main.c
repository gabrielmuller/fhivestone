#include "heuristics.h"
#include "node.h"

int main (void) {
    int values[] = {1, 10, 100, 1000, 1000000};
    int* table = generate_table(5, values);
    printf("%d, %d, %d\n", table[0x117], table[0x212], table[0x115]);

    Position a, b, c, d;
    a.x = 5;
    a.y = 3;
    b.x = 8;
    b.y = 9;
    c.x = 0;
    c.y = 0;
    d.x = 10;
    d.y = 11;
    Node* n = create_node(a);
    Node* m = create_node(b);
    Node* o = create_node(c);
    Node* p = create_node(d);
    Node** n_children = {&m, &o};
    Node** o_children = {&p};
    n.children = n_children;
    o.children = o_children;

    return 0;
}
