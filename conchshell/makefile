compiler: lex.yy.c
	g++ conchShell.cpp lexer.c lex.yy.c -ll -o shell

lex.yy.c: lexer.l
	lex lexer.l

clean:
	rm lex.yy.c shell