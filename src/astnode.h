/*
 * IFJ projekt 2019
 * **************************
 * Tým 82, varianta II
 * **************************
 * Autoři:
 * Zdeněk Kolba (xkolba01)
 * Tomáš Oplatek (xoplat01)
 * David Rubý (xrubyd00)
 * Petr Volf (xvolfp00)
 * **************************
 * astnode.h
 */

#ifndef IFJ_ASTNODE_H
#define IFJ_ASTNODE_H

#include <stdlib.h>
#include "tokens.h"
#include "symtable.h"

#define START_CAPACITY 10

typedef enum {
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
    VALUE,
    FLOAT_TO_INT, //PETER AS A FUCKING IDIOT. INT_TO_FLOAT
    FUNCTION_DEFINITION,
    FUNCITON_CALL,
    RETURN_VALUE,
    KEYWORD_PASS
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
    SymTable* symbol;
    NumberVal n;
    CondType condType;
    char* str_val;
    TypeValue arith_type;
    struct node** nodes;
};

typedef struct node ASTNode;

ASTNode* node_new();
void node_insert(ASTNode* node, ASTNode* new);
void free_tree(ASTNode* tree);
void print_tree(ASTNode* tree);
ASTNode** get_all_expressions(ASTNode* root, unsigned* len);
ASTNode** get_postorder(ASTNode* root, int* size);
ASTNode** get_preorder(ASTNode* root, int* size);


#endif //IFJ_ASTNODE_H
