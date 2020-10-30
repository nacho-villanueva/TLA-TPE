%{
    #include <stdio.h>
    #include <string.h>
    #include "y.tab.h"
%}

digit [0-9]
alpha [a-zA-Z]

%%
";"				return ENDL;
"="			 	return EQUAL;
":"			 	return COLON;
"+"			 	return PLUS;
"-"			 	return MINUS;
"*"			 	return TIMES;
"\\"				return DIVIDE;
"{"			 	return BRACKET_OPEN;
"}"			 	return BRACKET_CLOSE;
"int"		       		return INT_TYPE;
"float"		     		return FLOAT_TYPE;
"figure"		    	return FIGURE_TYPE;
"function"		  	return FUNCTION_TYPE;
"start"             return START_PROGRAM;
"end"               return END_PROGRAM;
{alpha}({alpha}|{digit})*   	{yylval.string = strdup(yytext); return IDENTIFIER;}
{digit}+		    	{yylval.number = atoi(yytext); return INTEGER;}
\"[^\"]*\"			{yylval.string = strdup(yytext); return STRING;}
-?{digit}+\.{digit}+		{yylval.decimal = atof(yytext); return STRING;};
\n			  	/* ignore end of line */;
[ \t]+		      		/* ignore whitespace */;


%%