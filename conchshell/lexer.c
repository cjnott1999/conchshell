#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

Token *head = NULL;

void addTokenToBuffer(TokenType type, char *identifier) {

    Token *cur = NULL;

    /* Walk the linked list */
    if (head == NULL) {
        head = (Token *)malloc(sizeof(Token));
        cur = head;
    } else {
        cur = head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        Token *nextValue = (Token *)malloc(sizeof(Token));
        cur->next = nextValue;
        cur = nextValue;
    }

    //Set the current type
    cur->type = type;
    if (identifier != NULL) {
        char *newIdentifier = (char *)malloc(strlen(identifier)+1);
        memset(newIdentifier, 0, strlen(identifier)+1);
        strcpy(newIdentifier, identifier);
        cur->identifier = newIdentifier;
    } 
    cur->next = NULL;

}

Token * lex(char *commnandArgument) {
    head = NULL;
    lexical_analysis(commnandArgument);
    return head;
}
