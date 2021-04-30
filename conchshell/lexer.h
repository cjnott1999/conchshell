#ifndef __LEXER_H__
#define __LEXER_H__

#include "tokens.h"

extern Token * lex(char *commandArgument);
extern void addTokenToBuffer(TokenType type, char *identifier);
extern int lexical_analysis(char *commandArgument);

#endif // __LEXER_H__
