%{
    #include <stdio.h>
    #include <stdlib.h>

    #include "lexer.h"
    #include "tokens.h"

%}

%%
[ \r\n]                     { }
exit                        {addTokenToBuffer(TOKEN_EXIT, yytext);}
cd                          {addTokenToBuffer(TOKEN_CD, yytext);}
history                     {addTokenToBuffer(TOKEN_HISTORY, yytext);}
[-a-zA-Z0-9\.\n\\]*         {addTokenToBuffer(TOKEN_WORD, yytext);}
\>                          {addTokenToBuffer(TOKEN_FILE_OUTPUT, yytext);}
\<                          {addTokenToBuffer(TOKEN_FILE_INPUT, yytext);}
!                           {addTokenToBuffer(TOKEN_BANG, yytext);}
.                           {addTokenToBuffer(TOKEN_WORD, yytext);}

%%


int lexical_analysis(char *commandArgument){
    YY_BUFFER_STATE stateBuffer = yy_scan_string(commandArgument);
    yy_switch_to_buffer(stateBuffer);
    yylex();
    yy_delete_buffer(stateBuffer);
    return 0;

}