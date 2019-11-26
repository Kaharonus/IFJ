//
// Created by petr on 26.11.19.
//

#ifndef IFJ_ASTNODE_H
#define IFJ_ASTNODE_H

#include <stdlib.h>
#include "tokens.h"

#define START_CAPACITY 10

typedef enum {
    STATEMENT,
    EXPRESSION,
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    INT_DIVISION,
    ASSIGNMENT,
    CONDITION,
    BLOCK,
    PROGRAM_ROOT,
    IDENTIFICATOR,
    IF_ELSE,
    WHILE_LOOP,
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_STRING,
    CONVERSION,
} NonTerm;

typedef enum {
    OP_NONE,
    OP_GR,
    OP_LS,
    OP_EQ,
    OP_NEQ,
    OP_GREQ,
    OP_LSEQ
} CondType;

struct node {
    NonTerm node_type;
    unsigned subnode_len;
    unsigned capacity;
    void* symbol;
    NumberVal n;
    CondType condType;
    char* str_val;
    struct node** nodes;
};

typedef struct node ASTNode;

ASTNode* node_new();
void node_insert(ASTNode* node, ASTNode* new);
void free_tree(ASTNode* tree);
void print_tree(ASTNode* tree);

#endif //IFJ_ASTNODE_H
