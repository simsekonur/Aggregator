%{
#define YYSTYPE double
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "functions.h"
extern double yylval;
%}
%%
[ \t] {;}
max return MAX;
min return MIN;
[+-]?[0-9]+(\.)?[0-9]+ {yylval = strtod(yytext, NULL); return NUMBER;}
"+" return PLUS;
"-" return MINUS;
"*" return MUL;
"/" return DIV;
"(" return OPENBRACKET;
")" return CLOSEBRACKET;
%%