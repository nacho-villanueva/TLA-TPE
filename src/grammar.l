%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    #include "y.tab.h"
%}

%option nounput noinput
%option yylineno

digit [0-9]
alpha [a-zA-Z]
boolean_primitive [true|false]
/* boolean_primitive (t|T)(r|R)(u|U)(e|E) */

%%
\/\/(.*) 						/* ignore single line comments */; 
\/\*(.|\n)*\*\/					/* ignore multi line comments */;	
";"								return ENDL;
"="								return EQUAL;
":"								return COLON;
"+"								return PLUS;
"-"								return MINUS;
"*"								return TIMES;
"/"							    return DIVIDE;
"%"                             return MODULE;
"&&"                            return AND;
"||"                            return OR;
"<"                             return LT;
">"                             return GT;
"<="                            return LE;
">="                            return GE;
"=="                            return EQ;
"!="                            return NEQ;
"("                             return OPEN;
")"                             return CLOSE;
"{"								return BRACKET_OPEN;
"}"								return BRACKET_CLOSE;
","                             return COMMA;
"."                             return DOT;
"const"                         return CONST;
"int"							return INT_TYPE;
"float"							return FLOAT_TYPE;
"string"                        return STRING_TYPE;
"figure"						return FIGURE_TYPE;
"boolean"                       return BOOLEAN_TYPE;
"while"                         return WHILE;
"if"                            return IF;
(S|s)(E|e)(T|t)(T|t)(I|i)(N|n)(G|g)(S|s) return SETTINGS_BLOCK;
(D|d)(R|r)(A|a)(W|w)           	return DRAW_BLOCK;
(E|e)(N|n)(D|d)       			return END_BLOCK; 
{alpha}({alpha}|{digit})*		{yylval.string = strdup(yytext); return IDENTIFIER; };
-?{digit}+						{yylval.number = atoi(yytext); return INTEGER;};
\"[^\"]*\"						{int len = strlen(yytext); yylval.string = strndup(yytext+1,len-2); return STRING;};
-?{digit}+\.{digit}+			{yylval.decimal = atof(yytext); return FLOAT;};
{boolean_primitive}             {   
                                    if(strcmp(yytext, "true") == 0){ 
                                        yylval.boolean = true; 
                                    } else if(strcmp(yytext, "false") == 0) {
                                        yylval.boolean = false;
                                    } return BOOLEAN; 
                                };
\n								/* ignore end of line */;
[ \t]+							/* ignore whitespace */;

%%