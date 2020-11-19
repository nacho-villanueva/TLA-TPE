 %{
    #include <stdio.h>
    #include <stddef.h>
    #include "nodes/node.h"

    void yyerror();
    int yylex();
    
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

%right EQUAL
%left PLUS MINUS
%left TIMES DIVIDE


%start start;

 %%

start: start_program;
start: line { addChildrenToNode(root, 1, $1); }
    | start line { addChildrenToNode(root, 1, $2); };

line: define_figure { $$ = newNode(VARIABLE_NODE, NULL, 1, $1); }
    | sum ENDL { $$ = $1; };

begin: line | begin line;

line: define_figure | sum ENDL | end_program;

end_program: END_PROGRAM ENDL  {printf("\t}\n}");};

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

int main() {
    Node * root = newNode(ROOT_NODE, NULL, 0, NULL);
    yyparse(root);
    printTree(root);
} 