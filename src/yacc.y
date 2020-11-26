%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>

    #include "src/nodes/node.h"
    #include "src/utils/logger.h"
    #include "src/u3d.h"

    bool hasError = false;
    void yyerror (struct Node * node, char const * msg);
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

%token SETTINGS_BLOCK DRAW_BLOCK END_BLOCK 
%token<character> ENDL
%token<character> EQUAL COLON
%token<character> PLUS MINUS TIMES DIVIDE
%token<string> INT_TYPE FLOAT_TYPE FIGURE_TYPE FUNCTION_TYPE
%token<string> IDENTIFIER
%token<number> INTEGER
%token<decimal> FLOAT
%token<string> STRING
%token<character> BRACKET_OPEN BRACKET_CLOSE
%token WHILE
%toke AND OR GT LT GE LE EQ NEQ

%type<node> definitions settings draw
%type<node> definition_list definition
%type<node> sum
%type<node> define_figure
%type<node> figure_atributes
%type<node> figure_atribute
%type<node> value
%type<node> code_block
%type<node> while
%type<node> conditional

//%right EQUAL
%left PLUS// MINUS
%left code_block
//%left TIMES DIVIDE


%start start


%%
start: definitions settings draw { addChildrenToNode(root, 3, $1, $2, $3); }
     | error definitions { yyerror(root, "Blocks must be declared in the following order. Definitions, Settings and Draw.\n"); }
     ;

definitions: /* empty */ { $$ = NULL; }
           | definition_list { $$ = $1; };
           ;

settings: /* empty */ { $$ = NULL; }
        | SETTINGS_BLOCK END_BLOCK { $$ = newNode(SETTINGS_NODE, NULL, 0); }
        ;

draw: /* empty */ { $$ = NULL; }
    | DRAW_BLOCK code_block END_BLOCK { $$ = newNode(DRAW_NODE, NULL, 0); }
    ;

definition_list: definition_list definition { addChildrenToNode($1, 1, $2); }
               | definition { $$ = newNode(DEFINITIONS_NODE, NULL, 1, $1); }
               ;

definition: define_figure { $$ = newNode(VARIABLE_NODE, NULL, 1, $1); }
          | sum ENDL { $$ = $1; };

define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atributes BRACKET_CLOSE {$$ = newNode(FIGURE_NODE, NULL, 1, $5);};

figure_atributes: figure_atributes figure_atribute { addChildrenToNode($1, 1, $2); }
                | figure_atribute {$$ = newNode(ATTRIBUTE_LIST_NODE, NULL, 1, $1); }
                ;

figure_atribute: IDENTIFIER COLON value ENDL { $$ = newNode(ATTRIBUTE_NODE, NULL, 0); };

value: INTEGER {$$ = newNode(INTEGER_CONSTANT_NODE, NULL, 0); }
    | FLOAT {$$ = newNode(FLOAT_CONSTANT_NODE, NULL, 0); }
    | STRING {$$ = newNode(STRING_CONSTANT_NODE, NULL, 0); }
    ;

sum: sum PLUS sum {$$ = newNode(PLUS_NODE, NULL, 2, $1, $3); }
    | INTEGER { $$ = newNode(INTEGER_CONSTANT_NODE, NULL, 0); }
    ; 

code_block: /* empty */
          | code_block code_block
          | while
          | if
          | variable_definition
          | function_call
          ;


conditional: conditional AND conditional
            | conditional OR conditional
            | expression LT expression
            | expression GT expression
            ;

while: WHILE '(' conditional ')' '{' code_block '}' {$$ = newNode(WHILE_NODE, NULL, 2, $3, $6)}; 


%%

int yywrap()
{
        return 1;
}

void yyerror (struct Node * node, char const * msg){
    logError(SYNTAX_ERROR, "%s (Line: %d)\n", msg, yylineno);
    hasError = true;
}

int main(int argc, char * argv[]) {
    initDebugLog();

    U3D * u3d = initU3D(argc, argv);
    if(u3d == NULL)
        return -1;

	yyin = getU3DInputFile(u3d);
    
    Node * root = newNode(ROOT_NODE, NULL, 0, NULL);
    yyparse(root);

    if(!hasError){
        parseNode(root);
        printTree(root);
    }

    compileU3D(u3d);

    closeU3D(u3d);
} 