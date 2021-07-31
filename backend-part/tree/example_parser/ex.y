%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE double
%}


%token NUMBER
%token PLUS MINUS DIV MUL OPENBRACKET CLOSEBRACKET
%token MAX MIN
%left PLUS MINUS
%left MUL DIV

%%
line
		: add_expr '\n' {printf("%f\n", $1);}
		;
add_expr
		: mul_expr
		| mul_expr PLUS add_expr {$$ = $1 + $3;}
		| mul_expr MINUS add_expr {$$ = $1 - $3;}
		;
mul_expr
		: func_call
		| func_call MUL mul_expr {$$ = $1 * $3;}
		| func_call DIV mul_expr {$$ = $1 / $3;}
		;
func_call
		: primary
		| MAX OPENBRACKET add_expr CLOSEBRACKET {$$ = max($3);}
		| MIN OPENBRACKET add_expr CLOSEBRACKET {$$ = min($3);}
		;
primary
		: NUMBER {$$ = $1;}
		| OPENBRACKET add_expr CLOSEBRACKET
		;
%%
void yyerror(const char *str) {
	fprintf(stderr, "%s\n", str);
}
int yywrap() {
	return 1;
}
double yylval;
int main() {
	yyparse();
	return 0;
}