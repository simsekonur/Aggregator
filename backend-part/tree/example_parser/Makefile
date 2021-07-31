all:
	lex ex.l;
	yacc -d ex.y;
	gcc lex.yy.c y.tab.c functions.c -o ex; 
