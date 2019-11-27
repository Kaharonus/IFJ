//
// Created by Petr on 26/11/2019.
//

#include "semantic_analyzer.h"

int check_semantics(ASTNode *tree) {
    int result = check_types(tree);
    if (result != 0) {
        return result;
    }
    return 0;
}

int check_types(ASTNode *tree) {
    unsigned exprs_len = 0;
    ASTNode** exprs = get_all_expressions(tree, &exprs_len);

    printf("nalezeno %u vyrazu", exprs_len);

    for (unsigned i = 0; i < exprs_len; i++) {
        ASTNode* expr = exprs[i];
    }

    free(exprs);
    return 0;
}
