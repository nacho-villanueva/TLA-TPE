%{
    #include <stdio.h>
    #include <stddef.h>
    #include <stdbool.h>

    #include "nodes/node.h"
    #include "utils/logger.h"

    bool hasError = false;
    void yyerror();
    int yylex();

    extern FILE *yyin;
    extern int yylineno;
%}

%parse-param {struct Node * root }

%union {
    int number;
    char * string;
    char character;
    float decimal;
    struct Node * node;
}

%token<string> START_PROGRAM
%token<string> END_PROGRAM
%token<character> ENDL
%token<character> EQUAL COLON
%token<character> PLUS MINUS TIMES DIVIDE
%token<string> INT_TYPE FLOAT_TYPE FIGURE_TYPE FUNCTION_TYPE
%token<string> IDENTIFIER
%token<number> INTEGER
%token<decimal> FLOAT
%token<string> STRING
%token<character> BRACKET_OPEN BRACKET_CLOSE

%type<node> line
%type<node> sum
%type<node> define_figure
%type<node> figure_atributes
%type<node> figure_atribute
%type<node> value

//%right EQUAL
%left PLUS// MINUS
//%left TIMES DIVIDE


%start start


%%

start: line { addChildrenToNode(root, 1, $1); }
    |  start line { addChildrenToNode(root, 1, $2); }
    |  start error { logError("Something is wrong"); }

line: define_figure { $$ = newNode(VARIABLE_NODE, NULL, 1, $1); }
    | sum ENDL { $$ = $1; }
    ;

define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atributes BRACKET_CLOSE {$$ = newNode(FIGURE_NODE, NULL, 1, $5);};

figure_atributes: figure_atributes figure_atribute { addChildrenToNode($1, 1, $2);}
                | figure_atribute {$$ = newNode(ATTRIBUTE_LIST_NODE, NULL, 1, $1);}
                ;

figure_atribute: IDENTIFIER COLON value ENDL { $$ = newNode(ATTRIBUTE_NODE, NULL, 0); };

value: INTEGER {$$ = newNode(INTEGER_CONSTANT_NODE, NULL, 0); }
    | FLOAT {$$ = newNode(FLOAT_CONSTANT_NODE, NULL, 0); }
    | STRING {$$ = newNode(STRING_CONSTANT_NODE, NULL, 0); }
    ;

sum: sum PLUS sum {$$ = newNode(PLUS_NODE, NULL, 2, $1, $3); }
    | INTEGER { $$ = newNode(INTEGER_CONSTANT_NODE, NULL, 0); }
    ; 

%%

int yywrap()
{
        return 1;
}

void yyerror(){
    logError("Syntax error (Line: %d)\n", yylineno);
    hasError = true;
}

int main(int argc, char * argv[]) {

    if(argc < 2) {
        logError("\033[0mu3d: \033[1;31mfatal error:\033[0m no input files\n");
        return -1;
    }
    FILE *file = fopen(argv[1], "r");
	if (!file) {
		logError("\033[0mu3d: \033[1;31merror:\033[0m No such file.\n");
		return -1;
	}

	yyin = file;

    initDebugLog();
    Node * root = newNode(ROOT_NODE, NULL, 0, NULL);
    yyparse(root);

    if(!hasError){
        parseNode(root);
        printTree(root);
    }

    fclose(file);
} 