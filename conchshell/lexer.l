%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "lexer.h"
    #include "tokens.h"

%}

%%
[ \t\n]                   { }
[a-zA-Z][a-zA-Z0-9]*      {addTokenToBuffer(TOKEN_WORD, yytext);}
-[a-zA-Z][a-zA-Z0-9]*     {addTokenToBuffer(TOKEN_FLAG, yytext);}
[[:digit:]]+              {printf("Integer %d\n", atoi(yytext)); }
\>                        {addTokenToBuffer(TOKEN_FILE_OUTPUT, yytext);}
\<                        {addTokenToBuffer(TOKEN_FILE_INPUT, yytext);}
!                         {addTokenToBuffer(TOKEN_BANG, yytext);}


%%


int lexical_analysis(char *commandArgument){
    YY_BUFFER_STATE stateBuffer = yy_scan_string(commandArgument);
    yy_switch_to_buffer(stateBuffer);
    yylex();
    yy_delete_buffer(stateBuffer);
    return 0;

}