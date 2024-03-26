#include "netcoding.h"

two_xor_node *two_xor_node_constructor() {
    two_xor_node *node = (two_xor_node *)malloc(sizeof(two_xor_node));
    return node;
}

void two_xor_node_destructor(two_xor_node *node) { free(node); }