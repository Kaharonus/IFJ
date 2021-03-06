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
 * syntax_stack.h
 */

#ifndef IFJ_SYNTAX_STACK_H
#define IFJ_SYNTAX_STACK_H

#include "lex_analyzer.h"
#include "astnode.h"

#define SYNTAX_STACK_MAX_SIZE 100

typedef enum {
    SYNTAX_GREATER = '>',
    SYNTAX_EQUAL = '=',
    SYNTAX_LESSER = '<',
    SYNTAX_TERM = 'i',
    SYNTAX_EXPR = 'E',
    SYNTAX_END = '$',
    SYNTAX_EMPTY = ' '
} SSValue;

typedef struct {
    SSValue type;
    Token t;
    ASTNode* node;
} SSData;

typedef struct {
    unsigned index;
    SSData data[SYNTAX_STACK_MAX_SIZE];
} SyntaxStack;

void syntax_stack_push(SyntaxStack* ss, SSData data);
void syntax_stack_init(SyntaxStack* ss);
void syntax_stack_shift(SyntaxStack* ss, unsigned index);
void syntax_stack_pop(SyntaxStack* ss);
void syntax_stack_free_nodes(SyntaxStack* ss);
bool syntax_stack_empty(SyntaxStack* ss);
SSData syntax_stack_nearest_term(SyntaxStack* ss, unsigned* loc);
SSData syntax_stack_top(SyntaxStack*ss);

#endif //IFJ_SYNTAX_STACK_H
