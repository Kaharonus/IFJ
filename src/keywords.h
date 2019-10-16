#ifndef IFJ_KEYWORDS_H
#define IFJ_KEYWORDS_H

#include <string.h>

typedef enum {
        DEF,
        ELSE,
        IF,
        NONE,
        PASS,
        RETURN,
        WHILE,
        NON_KEYWORD
} KeywordValue;

extern const char KEYWORDS[][10];

KeywordValue is_keyword(char * input);

#endif //IFJ_KEYWORDS_H
