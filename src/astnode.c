//
// Created by petr on 26.11.19.
//

#include "astnode.h"

ASTNode* node_new() {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    node->node_type = PROGRAM_ROOT;
    node->subnode_len = 0;
    node->capacity = START_CAPACITY;
    node->n.i = 0;
    node->str_val = NULL;
    node->symbol = NULL;
    node->condType = OP_NONE;
    node->nodes = malloc(START_CAPACITY * sizeof(ASTNode*));
    if (node->nodes == NULL) {
        free(node);
        return NULL;
    } else {
        return node;
    }
}

void node_insert(ASTNode* node, ASTNode* new) {
    if ((node->subnode_len + 1) > node->capacity) {
        node->nodes = realloc(node->nodes, node->capacity * 10);
        node->capacity *= 10;
    }
    node->nodes[node->subnode_len] = new;
    node->subnode_len += 1;
}

void free_tree(ASTNode* tree) {
    if (tree == NULL) {
        return;
    }

    // TODO: dodelat
    for (unsigned i = 0; i < tree->subnode_len; i++) {
        free_tree(tree->nodes[i]);
    }
    free(tree->nodes);
    free(tree);
}

ASTNode* node_iter_next(ASTIterator *iter) {
     return NULL;
}

const char *node_types[100] = {
    "STATEMENT",
    "EXPRESSION",
    "ADDITION",
    "SUBTRACTION",
    "MULTIPLICATION",
    "DIVISION",
    "INT_DIVISION",
    "ASSIGNMENT",
    "CONDITION",
    "BLOCK",
    "PROGRAM_ROOT",
    "IDENTIFICATOR",
    "IF_ELSE",
    "WHILE_LOOP",
    "VALUE_INT",
    "VALUE_FLOAT",
    "VALUE_STRING",
    "CONVERSION",
    NULL
};

void print_node_name(ASTNode* tree, int indent){
    for(int i = 0; i < indent*2; i++){
        printf(" ");
    }
    printf("%s\n", node_types[tree->node_type]);
}
void print_tree_private(ASTNode* tree, int indent){
    indent += 1;
    print_node_name(tree, indent);
    for(unsigned int i = 0; i < tree->subnode_len; i++){
        print_tree_private(tree->nodes[i], indent);
    }
}

void print_tree(ASTNode* tree){
    printf("Vypis binarniho stromu:\n");
    print_tree_private(tree, -1);
    printf("Konec stromu\n");
}