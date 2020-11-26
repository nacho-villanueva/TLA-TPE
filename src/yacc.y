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

%type<node> declaration_list definition
%type<node> define_figure
%type<node> figure_atributes
%type<node> figure_atribute
%type<node> value
%type<node> block_list
%type<node> block

//%right EQUAL
//%left PLUS// MINUS
//%left TIMES DIVIDE


%start start


%%
start: /* empty */                       { /* Do Nothing */} 
     |  declaration_list block_list      { addChildrenToNode(root, 1, $1); }
     |  block_list
     |  declaration_list                 { addChildrenToNode(root, 1, $1); }
     ;

block_list: block_list block { addChildrenToNode(root, 1, $2); }
      | block { addChildrenToNode(root, 1, $1); }
      ;

block: SETTINGS_BLOCK END_BLOCK { $$ = newNode(SETTINGS_NODE, NULL, 0); }
     | DRAW_BLOCK END_BLOCK { $$ = newNode(DRAW_NODE, NULL, 0); }
     ;

declaration_list: declaration_list definition { addChildrenToNode($1, 1, $2); }
               | definition { $$ = newNode(DEFINITIONS_NODE, NULL, 1, $1); }
               ;

definition: define_figure { $$ = newNode(VARIABLE_NODE, NULL, 1, $1); };

define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atributes BRACKET_CLOSE {$$ = newNode(FIGURE_NODE, NULL, 1, $5);};

figure_atributes: figure_atributes figure_atribute { addChildrenToNode($1, 1, $2); }
                | figure_atribute {$$ = newNode(ATTRIBUTE_LIST_NODE, NULL, 1, $1); }
                ;

figure_atribute: IDENTIFIER COLON value ENDL { $$ = newNode(ATTRIBUTE_NODE, NULL, 0); };

value: INTEGER {$$ = newNode(INTEGER_CONSTANT_NODE, NULL, 0); }
    | FLOAT {$$ = newNode(FLOAT_CONSTANT_NODE, NULL, 0); }
    | STRING {$$ = newNode(STRING_CONSTANT_NODE, NULL, 0); }
    ;

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
        compileU3D(u3d);
    }

    printTree(root);

    closeU3D(u3d);
} 