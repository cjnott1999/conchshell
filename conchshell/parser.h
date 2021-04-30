#ifndef __PARSER_H__
#define __PARSER_H__

#include "ast.h"
#include "tokens.h"

AST *parse(Token *tokens);

extern Token scanToken();
extern void putBackToken(Token t);
extern int expectToken(TokenType t);
extern void setupTokenBuffer(Token *srcTokenBuffer);

#endif // __PARSER_H__
