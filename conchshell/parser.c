#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "recursive_descent.h"

/* TODO: actually implement */
AST *parse(Token *tokens) {
    setupTokenBuffer(tokens);
    AST *returnValue = (AST *)malloc(sizeof(AST));

    int checkParse = parse_P();
    printf("recursive descent returned %d", checkParse);
    return returnValue;
}

static Token *token_buffer;
static Token *current_token;

Token scanToken() {
    /* get current value, and advance pointer for future use */
    return *current_token++;
}

void putBackToken(Token t) {
    /* move the pointer backward, then set the value of its contents */
    *--current_token = t;
}

int expectToken(TokenType t) {
    /* check if the current token matches. If so, return true. */
    if (current_token->type == t) {
        current_token++;
        return 1;
    }
    return 0;
}

void setupTokenBuffer(Token *srcTokenBuffer) {

    //Count the number of tokens 

    int tokenCount = 0;
    Token *cur = srcTokenBuffer;
    while (cur != NULL){
        ++tokenCount;
        cur = cur -> next;
    }
    tokenCount += 1; //For EOL


    token_buffer = (Token *)malloc(tokenCount * sizeof(Token));
    current_token = token_buffer;

    cur = srcTokenBuffer;
    while (cur != NULL){
        memcpy(current_token, cur, sizeof(Token));
        current_token++;
        cur = cur->next;
    }

    Token eol;
    eol.type = TOKEN_EOL;
    eol.identifier = NULL;

    memcpy(current_token, &eol, sizeof(Token));

    current_token = token_buffer;
}
