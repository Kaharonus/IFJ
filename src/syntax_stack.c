//
// Created by petr on 30.10.19.
//

#include "syntax_stack.h"

void syntax_stack_push(SyntaxStack* ss, SSData data) {
    ss->data[ss->index] = data;
    //ss->tokens[ss->index] = t;
    ss->index++;
}

void syntax_stack_init(SyntaxStack* ss) {
    ss->index = 1;
    SSData data;
    data.type = SYNTAX_END;
    ss->data[0] = data;
}

void syntax_stack_pop(SyntaxStack* ss) {
    if (!syntax_stack_empty(ss)) {
        ss->index--;
    }
}

bool syntax_stack_empty(SyntaxStack* ss) {
    return ss->index == 0;
}

SSData syntax_stack_nearest_term(SyntaxStack* ss, unsigned* loc) {
    for (unsigned i = ss->index - 1; i >= 0; i--) {
        if (ss->data[i].type != SYNTAX_EXPR) {
            if (loc != NULL) {
                *loc = i;
            }
            return ss->data[i];
        }
    }
}

void syntax_stack_shift(SyntaxStack* ss, unsigned index) {
    for (unsigned i = SYNTAX_STACK_MAX_SIZE - 1; i > index; i--) {
        ss->data[i] = ss->data[i - 1];
    }
    ss->data[index + 1].type = SYNTAX_LESSER;
    ss->index++;
}

