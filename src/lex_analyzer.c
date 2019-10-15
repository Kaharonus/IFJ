//
// Created by tomas on 14.10.19.
//

#include "lex_analyzer.h"


//Načte z stdin další alfanumerické slovo (může obsahovat i '_')
int read_next_word(FILE* source, char* word, int size){
    if(word == NULL){
        //throw
    }
    int counter = 0;
    char c = 0;
    //Všechny znaky které můžou obsahovat identifier.
    while(((c = getc(source)) >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c<= '9') || c == '_'){
        if(counter-1 == size){
            size += 128;
            word = realloc(word , size*sizeof(char));
        }
        word[counter] = c;
        counter++;
    }
    return counter;
}

void handle_word(FILE* source ,Token *token){
    char* word = calloc(256, sizeof(char));
    if(word == NULL){
        //throw
    }
    int len = read_next_word(source, word, 256);
    token->keywordValue = is_keyword(word);
    if(token->keywordValue != NON_KEYWORD){
        printf("Keyword: %s\n",word);
        token->type = KEYWORD;
        free(word);
        return;
    }
    free(word);
}

int count_spaces(FILE* source) {
    int spaces = 0;
    char c;
    do {
        c = (char)getc(source);
        if (c == ' ') {
            spaces++;
        } else {
            ungetc(c, source);
            break;
        }
    } while (true);
    return spaces;
}

// Vraci:
// 0 - kdyz jen zkonzumuje mezery a nechce vratit token
// 1 - kdyz chce vratit token
// 2 - kdyz najde chybu
int handle_indent(FILE* source, IndentStack *is, Token* t){
    int indent = count_spaces(source);

    // je stejny indent
    if (indent == stack_top(is)) {
        return 0;
    } else if (indent > stack_top(is)) {
        // je vetsi indent nez predtim
        stack_push(is, indent);
        t->type = INDENT;
        return 1;
    } else {
        // je mensi hodnota nez predtim
        stack_pop(is);
        if (stack_top(is) < indent) {
            return 2;
        } else if (stack_top(is) > indent) {
            for (int i = 0; i < indent; i++) {
                ungetc(' ', source);
            }
        }
        t->type = DEDENT;
        return 1;
    }
}

void handle_singleline_comments(FILE * source) {
    do {
        char c = (char)getc(source);
        if (c == EOF || c == '\n') {
            ungetc(c, source);
            return;
        }
    } while (true);
}

void handle_eof(FILE* source, IndentStack* is, Token* t) {
    if (stack_top(is) == 0) {
        t->type = END_OF_FILE;
    } else {
        stack_pop(is);
        t->type = DEDENT;
        ungetc(EOF, source);
    }
}

bool is_num(char c) {
    return (c >= '0' && c <= '9');
}

void handle_number(FILE* source, Token* t) {
    char c;
    // 20 cisel bude snad stacit, kdyztak se prida
    int var_len = 20;
    char* variable = malloc(var_len);
    int num_len = 0;
    // precist cislo
    bool dot = false;
    do {
        c = (char)getc(source);
        if (is_num(c) || c == '.') {
            if (num_len + 1 == var_len) {
                variable = realloc(variable, var_len * 10);
                var_len*= 10;
            }
            if (c == '.') {
                dot = true;
            }
            variable[num_len] = c;
            num_len++;
        } else if (c == ' ') {
            continue;
        } else {
            ungetc(c, source);
            break;
        }
    } while(true);
    variable[num_len + 1] = '\0';
    // tady uz jsou prectene cisla
    if (dot) {
        double d = strtod(variable, NULL);
        t->numberVal.d = d;
        t->type = FLOAT;
    } else {
        int i = (int)strtol(variable, NULL, 10);
        t->numberVal.i = i;
        t->type = INT;
    }
}

Token get_next_token(FILE* source, IndentStack* is){
    Token t;
    t.type = ERROR;
    t.keywordValue = NON_KEYWORD;
    char c;
    // cteni souboru znak po znaku
    do {
        c = (char)getc(source);
        switch (c) {
            case '\n':;
            // nebude fungovat poradne dokud nebudem rozpoznavat vsechno
            //case ' ':;
                int value = handle_indent(source, is, &t);
                if (value == 0) {
                    continue;
                } else if (value == 1) {
                    return t;
                } else {
                    t.type = ERROR;
                    return t;
                }
            case '"':
                break;
            case '#':
                handle_singleline_comments(source);
                break;
            case EOF:
                handle_eof(source, is, &t);
                return t;
            default:
                //Všechny znaky kterými může začínát indentifier
                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
                    ungetc(c, source);
                    handle_word(source, &t);
                    return t;
                } else if (is_num(c)) {
                    ungetc(c, source);
                    handle_number(source, &t);
                    return t;
                }
                break;
        }
    } while (t.type == END_OF_FILE);

    return t;
}
