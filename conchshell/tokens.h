#ifndef __TOKENS_H__
#define __TOKENS_H__


//Define the Token Type and the Valid tokens
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_FLAG,
    TOKEN_FILE_OUTPUT,
    TOKEN_FILE_INPUT, 
    TOKEN_BANG,
    TOKEN_EOL
} TokenType;

typedef struct t_token {
    TokenType type;

    char *identifier;
    struct t_token *next;
} Token;

#endif // __TOKENS_H__
