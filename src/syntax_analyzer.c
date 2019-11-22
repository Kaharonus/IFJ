#include "syntax_analyzer.h"
#include "syntax_stack.h"

char token_type[][100] = {
        "ID",
        "ASSIGN",
        "GREATER",
        "LESSER",
        "GREATER_OR_EQ",
        "LESSER_OR_EQ",
        "EQ",
        "NON_EQ",
        "OPEN_PARENTHES",
        "CLOSE_PARENTHES",
        "INT",
        "BOOL",
        "FLOAT",
        "STRING",
        "KEYWORD",
        "INDENT",
        "DEDENT",
        "END_OF_FILE",
        "END_OF_LINE",
        "NIL",
        "ERROR",
        "COLON",
        "COMMA",
        "ADD",
        "SUB",
        "MUL",
        "DIV",
        "DOUBLE_DIV"
};


SSValue parse_table[9][9] = {
    //            +    -    *    /    //   (    )   ID   end
    /* + */     {'>', ' ', '<', '<', '<', '<', '>', '<', '>'},
    /* - */     {' ', '>', '<', '<', '<', '<', '>', '<', '>'},
    /* * */     {'>', '>', '>', ' ', ' ', '<', '>', '<', '>'},
    /* / */     {'>', '>', ' ', '>', ' ', '<', '>', '<', '>'},
    /* // */    {'>', '>', ' ', ' ', '>', '<', '>', '<', '>'},
    /* ( */     {'<', '<', '<', '<', '<', '<', '=', '<', ' '},
    /* ) */     {'>', '>', '>', '>', '>', ' ', '>', ' ', '>'},
    /* ID */    {'>', '>', '>', '>', '>', ' ', '>', ' ', '>'},
    /* end */   {'<', '<', '<', '<', '<', '<', ' ', '<', ' '},
};

//Pravidla pro PSA
// E => E+E
// E => E-E
// E => E*E
// E => E/E
// E => E//E
// E => (E)
// E => ID

int check_rule(SyntaxStack* ss) {
    if (ss->data[ss->index - 1] == SYNTAX_TERM && ss->data[ss->index - 2] == SYNTAX_LESSER) {
        syntax_stack_pop(ss);
        syntax_stack_pop(ss);
        Token t;
        syntax_stack_push(ss, SYNTAX_EXPR, t);
        return 0;
    }
    return 1;
}

void free_tree(ASTNode* tree) {
    // TODO: dodelat
    free(tree);
}

int convert_token_to_table_index(Token t) {
    switch (t.type) {
        case ADD:
            return 0;
        case SUB:
            return 1;
        case MUL:
            return 2;
        case DIV:
            return 3;
        case DOUBLE_DIV:
            return 4;
        case OPEN_PARENTHES:
            return 5;
        case CLOSE_PARENTHES:
            return 6;
        case ID:
        case INT:
        case FLOAT:
        case STRING:
            return 7;
        case END_OF_LINE:
        case END_OF_FILE:
        case COLON:
            return 8;
        default:
            return -1;
    }
}

void node_init(ASTNode* node) {
    //node->node_type = 0;
    node->subnode_len = 0;
    node->capacity = 10;
    node->nodes = malloc(10 * sizeof(ASTNode));
}

void node_insert(ASTNode* node, ASTNode new) {
    if ((node->subnode_len + 1) > node->capacity) {
        node->nodes = realloc(node->nodes, node->capacity * 10);
        node->capacity *= 10;
    }
    node->nodes[node->subnode_len] = new;
    node->subnode_len += 1;
}

bool check_function_call(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    printf("kontrola volani funkce\n");
    return true;
}

bool check_expression(ASTNode* tree, Scanner* s) {
     printf("kontrola vyrazu\n");

    // zkontrolovat jestli neni volana funkce
    Token t = get_next_token(s);
    if (t.type == ID) {
        Token par = get_next_token(s);
        if (t.type == OPEN_PARENTHES) {
            scanner_unget(s, par);
            scanner_unget(s, t);
            return check_function_call(tree, s);
        } else {
            scanner_unget(s, par);
            scanner_unget(s, t);
        }
    } else {
        scanner_unget(s, t);
    }

    SyntaxStack ss;
    syntax_stack_init(&ss);

    t = get_next_token(s);

    // TODO: dodelat
    do {
        Token a;
        SSValue sv = syntax_stack_nearest_term(&ss, &a);

        int A = convert_token_to_table_index(a);
        if (sv == SYNTAX_END) {
            A = 8;
        }

        //t = get_next_token(s);
        int B = convert_token_to_table_index(t);

        switch (parse_table[A][B]) {
            case SYNTAX_GREATER:
                printf("syntax_greater\n");
                if (check_rule(&ss)) {
                    return 1;
                }
                break;
            case SYNTAX_EQUAL:
                printf("syntax_equal\n");
                syntax_stack_push(&ss, SYNTAX_TERM, t);
                t = get_next_token(s);
                break;
            case SYNTAX_LESSER:
                printf("syntax_lesser\n");
                Token throwaway;
                syntax_stack_push(&ss, SYNTAX_LESSER, throwaway);
                syntax_stack_push(&ss, SYNTAX_TERM, t);
                t = get_next_token(s);
                break;
            case SYNTAX_EMPTY:
            default:
                return 1;
        }
    } while (!(t.type == END_OF_LINE || t.type == END_OF_FILE  || t.type == COLON) || syntax_stack_nearest_term(&ss, NULL) != SYNTAX_END);
    return 0;
}

bool check_assignment(ASTNode* tree, Scanner* s, char* left_side) {
    //TODO: dodelat
    printf("kontrola prirazeni\n");
    if (check_expression(tree, s)) {
        return true;
    } else {
        return false;
    }
}

bool check_if(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return true;
}

bool check_while(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    return true;
}

bool check_definition(ASTNode* tree, Scanner* s) {
    //TODO: dodelat
    Token t = get_next_token(s);
    if (t.type != ID) {

    }

    return true;
}


// TODO: definice funkce nemuze byt v ifu / whilu

/// Vraci   0 - kdyz nenastala chyba
///         1 - kdyz nastala lexikalni chyba
///         2 - kdyz nastala syntakticka chyba
///         3 - kdyz nastal konec souboru
///         4 - kdyz narazi na 'DEDENT'
int check_block(ASTNode* tree, Scanner* s) {
    Token t = get_next_token(s);

    printf("kontrola bloku\n");

    switch (t.type) {
        case ID:;
            char* id = t.stringValue;
            Token after = get_next_token(s);
            switch (after.type) {
                case ASSIGN:
                    return check_assignment(tree, s, id);
                case OPEN_PARENTHES:
                    scanner_unget(s, after);
                    scanner_unget(s, t);
                    return check_function_call(tree, s);
                default:
                    return 2;
            }
        case KEYWORD:
            switch (t.keywordValue) {
                case IF:
                    return check_if(tree, s);
                case WHILE:
                    return check_while(tree, s);
                default:
                    return 2;
            }
        case STRING:
            //TODO: Poresit multiline stringy
            return 2;
        case END_OF_FILE:
            return 3;
        case DEDENT:
            return 4;
        default:
            return 2;
    }
}

int check_root_block(ASTNode* tree, Scanner *s) {
    Token t = get_next_token(s);
    switch (t.type) {
        case KEYWORD:
            if (t.keywordValue  == DEF) {
                return check_definition(tree, s);
            }
        default:
            scanner_unget(s, t);
            return check_block(tree, s);
    }
}

/// Vraci derivacni strom, ktery je potom potreba uvolnit
ASTNode* get_derivation_tree(FILE *source) {
    Scanner s;
    scanner_init(&s, source);

    ASTNode* root = (ASTNode*)malloc(sizeof(ASTNode));
    node_init(root);
    root->node_type = PROGRAM_ROOT;

    while (true) {
        int result = check_root_block(root ,&s);
        switch (result) {
            case 1:
                fprintf(stderr, "nastala lexikalni chyba\n");
                free_tree(root);
                return NULL;
            case 2:
                fprintf(stderr, "nastala syntakticka chyba\n");
                free_tree(root);
                return NULL;
            case 3:
                break;
            default:
                continue;
        }
    }
}
