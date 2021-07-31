%require "3.2"
%define api.pure full
%param {yyscan_t scanner}
%parse-param {AggParser& aggParser}

%code top {
	#include <stdio.h>
	#include <stdlib.h>
	#include <math.h>
	#include <string>
	#include <sstream>
	#include <memory>
	#include <iostream>
	#include "AggExceptions.hpp"
}
%code requires {
	typedef void* yyscan_t;
	#include <vector>
    #include "AggParser.hpp"
}
%code {
	int yylex(YYSTYPE* yylvalp, yyscan_t scanner);
	void yyerror(yyscan_t unused, AggParser _unused, const char* s);
	double getExpValue(AggParser& parser, AggExp* exp);
}

%union {
	double fval;
	char* sval;
	AggExp* expval;
    std::vector<AggExp*>* vectorval;
	Condition* condval;
}

%token<fval> T_FLOAT
%token<sval> T_STRING
%token<sval> T_COMPARE

%token T_PLUS T_MINUS T_MULTIPLY T_DIVIDE T_LEFT T_RIGHT T_POWER T_LOG T_SQRT T_COMMA T_SEMICOLON

%left T_PLUS T_MINUS
%left T_MULTIPLY T_DIVIDE
%right T_POWER

%type<expval> expression
%type<expval> function_call
%type<expval> identifier
%type<vectorval> expression_list;
%type<condval> condition;

%start result

%%

result: 
	expression {
		aggParser.setResult($1->getNumber());
		delete $1;
	}
;

expression: T_FLOAT {
		$$ = new AggExp($1);
	}
	| identifier
	| function_call
	| expression T_PLUS expression {
		double e1 = getExpValue(aggParser, $1);
		double e2 = getExpValue(aggParser, $3);
		$$ = new AggExp(e1 + e2);
		delete $1;
		delete $3;
	}
	| expression T_MINUS expression	{
		double e1 = getExpValue(aggParser, $1);
		double e2 = getExpValue(aggParser, $3);
		$$ = new AggExp(e1 - e2);
		delete $1;
		delete $3;
	}
	| expression T_MULTIPLY expression {
		double e1 = getExpValue(aggParser, $1);
		double e2 = getExpValue(aggParser, $3);
		$$ = new AggExp(e1 * e2);
		delete $1;
		delete $3;
	}
	| expression T_DIVIDE expression {
		double e1 = getExpValue(aggParser, $1);
		double e2 = getExpValue(aggParser, $3);
		if (e2 == 0.0) 
			throw parser_exception("Division by zero.");
		$$ = new AggExp(e1 / e2);
		delete $1;
		delete $3;
	}
	| expression T_POWER expression {
		double e1 = getExpValue(aggParser, $1);
		double e2 = getExpValue(aggParser, $3);
		$$ = new AggExp( pow(e1, e2) );
		delete $1;
		delete $3;
	}
	| T_LOG T_LEFT expression T_COMMA expression T_RIGHT {
		double e1 = getExpValue(aggParser, $3);
		double e2 = getExpValue(aggParser, $5);
		$$ = new AggExp( log(e1) / log(e2) );
		delete $3;
		delete $5;
	}
	| T_SQRT T_LEFT expression T_RIGHT {
		double e1 = getExpValue(aggParser, $3);
		$$ = new AggExp( sqrt(e1) );
		delete $3;
	}
	| T_LEFT expression T_RIGHT {
		$$ = $2;
	}
;

identifier: T_STRING {
		$$ = new AggExp($1);
		free($1);
	}
;

function_call: T_STRING T_LEFT expression_list T_RIGHT {
		double res = aggParser.apply($1, *$3, Condition(Comp::none, 0));
		$$ = new AggExp(res);
		free($1);
		std::vector<AggExp*>& list = *$3;
		for (AggExp* exp: list) {
			delete exp;
		}
		delete $3;
	}
	| T_STRING T_LEFT expression_list condition T_RIGHT {
		double result = aggParser.apply($1, *$3, *$4);
		$$ = new AggExp(result);
		free($1);
		std::vector<AggExp*>& list = *$3;
		for (AggExp* exp: list) {
			delete exp;
		}
		delete $3;
		delete $4;
	}
;

expression_list: 
    expression {
        std::vector<AggExp*>* e = new std::vector<AggExp*>();
        e->push_back($1);
        $$ = e;
    }
    | expression T_COMMA expression_list {
        $3->push_back($1);
        $$ = $3;
    } 
;

condition: T_SEMICOLON T_COMPARE expression {
	std::string compStr($2);
	std::cout << compStr << std::endl;
	free($2);
	Comp::Comparator comp;
	if ( !compStr.compare("<"))
		comp = Comp::lt;
	else if ( !compStr.compare("<=") || !compStr.compare("=<"))
		comp = Comp::lte;
	else if ( !compStr.compare(">"))
		comp = Comp::gt;
	else if ( !compStr.compare("=>") || !compStr.compare(">="))
		comp = Comp::gte;
	else if ( !compStr.compare("!="))
		comp = Comp::ne;
	else {
		std::stringstream stream;
		stream << "\"" << $2 << "\"" << " is not a valid comparator.";
		throw parser_exception(stream.str());
	}
	$$ = new Condition(comp, $3->getNumber());
	delete $3;
}
;

%%

void yyerror(yyscan_t unused, AggParser _unused, const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}

double getExpValue(AggParser& parser, AggExp* exp) {
	if (exp->isNumber()) {
		return exp->getNumber();
	}
	else {
		return parser.getNumberVariable(exp->getName());
	}
}