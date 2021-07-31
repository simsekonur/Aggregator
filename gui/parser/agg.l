%option noyywrap
%option reentrant bison-bridge

%{
#include <stdio.h>
#include "agg.tab.hpp"
#include "AggExp.hpp"
%}

%%

[ \t\n]	; // ignore all whitespace
"log"       {/*printf("LOG")*/; return T_LOG;}
"sqrt"      {/*print("SQRT");*/ return T_SQRT;}
","     {/*printf(",");*/ return T_COMMA;}
";"     {/*printf(";");*/ return T_SEMICOLON;}
"+"		{/*printf("+")*/; return T_PLUS;}
"-"		{/*printf("-")*/; return T_MINUS;}
"*"		{/*printf("*")*/; return T_MULTIPLY;}
"/"		{/*printf("/")*/; return T_DIVIDE;}
"("		{/*printf("(")*/; return T_LEFT;}
")"		{/*printf(")")*/; return T_RIGHT;}
"^"     {/*printf("^")*/; return T_POWER;}
"<"     {yylval->sval = strdup(yytext); return T_COMPARE;}
"<="    {yylval->sval = strdup(yytext); return T_COMPARE;}
">"     {yylval->sval = strdup(yytext); return T_COMPARE;}
">="    {yylval->sval = strdup(yytext); return T_COMPARE;}
"!="    {yylval->sval = strdup(yytext); return T_COMPARE;}

\.[0-9]+ {
    yylval->fval = atof(yytext);
    return T_FLOAT;
}

[0-9]+\.?([0-9]*)? 	{
    //printf("%s", "NUMBER");
    yylval->fval = atof(yytext);
    return T_FLOAT;
}

[a-zA-Z][a-zA-Z0-9]* {
    //printf("%s", "STRING");
    yylval->sval = strdup(yytext);
    return T_STRING;
}

%%