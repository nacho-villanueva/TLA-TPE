%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
%}

%option nounput noinput
%option yylineno

digit [0-9]
alpha [a-zA-Z]

%%
\/\/(.*) 						/* ignore single line comments */; 
\/\*(.|\n)*\*\/					/* ignore multi line comments */;	
";"								return ENDL;
"="								return EQUAL;
":"								return COLON;
"+"								return PLUS;
"-"								return MINUS;
"*"								return TIMES;
"\\"							return DIVIDE;
"{"								return BRACKET_OPEN;
"}"								return BRACKET_CLOSE;
"int"							return INT_TYPE;
"float"							return FLOAT_TYPE;
"figure"						return FIGURE_TYPE;
"function"						return FUNCTION_TYPE;
(S|s)(E|e)(T|t)(T|t)(I|i)(N|n)(G|g)(S|s) return SETTINGS_BLOCK;
(D|d)(R|r)(A|a)(W|w)           	return DRAW_BLOCK;
(E|e)(N|n)(D|d)       			return END_BLOCK; 
{alpha}({alpha}|{digit})*		{yylval.string = strdup(yytext); return IDENTIFIER; }
{digit}+						{yylval.number = atoi(yytext); return INTEGER;}
\"[^\"]*\"						{yylval.string = strdup(yytext); return STRING;}
-?{digit}+\.{digit}+			{yylval.decimal = atof(yytext); return STRING;};
\n								/* ignore end of line */;
[ \t]+							/* ignore whitespace */;

%%