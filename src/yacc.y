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
    bool boolean;
}

%token SETTINGS_BLOCK DRAW_BLOCK END_BLOCK 
%token<character> ENDL
%token<character> EQUAL COLON
%token<character> PLUS MINUS TIMES DIVIDE MODULE
%token<string> INT_TYPE FLOAT_TYPE FIGURE_TYPE FUNCTION_TYPE
%token<string> IDENTIFIER
%token<number> INTEGER
%token<decimal> FLOAT
%token<string> STRING
%token<boolean> BOOLEAN
%token<character> BRACKET_OPEN BRACKET_CLOSE
%token WHILE
%token IF
%token AND OR GT LT GE LE EQ NEQ

%type<node> /*definitions settings*/ draw
//%type<node> definition_list definition
//%type<node> sum
//%type<node> define_figure
//%type<node> figure_atributes
//%type<node> figure_atribute
//%type<node> value
%type<node> numeric_value
%type<node> string_value
%type<node> boolean_value
%type<node> if
%type<node> code_block
%type<node> conditional
%type<node> numeric_expression
%type<node> string_expression
%type<node> boolean_expression


//%right EQUAL
%left PLUS// MINUS
//%left code_block
//%left TIMES DIVIDE
%left AND
%left OR
//%left GT LT GE LE EQ NEQ

%start start


%%
start: draw;

draw: /* empty */ { $$ = NULL; }
    | DRAW_BLOCK code_block END_BLOCK { $$ = newNode(DRAW_NODE, NULL, 0); }
    ;

/* definition_list: definition_list definition { addChildrenToNode($1, 1, $2); }
               | definition { $$ = newNode(DEFINITIONS_NODE, NULL, 1, $1); }
               ;

definition: define_figure { $$ = newNode(VARIABLE_NODE, NULL, 1, $1); }
          | sum ENDL { $$ = $1; }; */

/* define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atributes BRACKET_CLOSE {$$ = newNode(FIGURE_NODE, NULL, 1, $5);};

figure_atributes: figure_atributes figure_atribute { addChildrenToNode($1, 1, $2); }
                | figure_atribute {$$ = newNode(ATTRIBUTE_LIST_NODE, NULL, 1, $1); }
                ;

figure_atribute: IDENTIFIER COLON value ENDL { $$ = newNode(ATTRIBUTE_NODE, NULL, 1, $3); }; */

/* value: numeric_value { $$ = $1; };
     | string_value { $$ = $1; };
     | boolean_value { $$ = $1; };
     ; */

numeric_value: INTEGER {$$ = newNode(INTEGER_CONSTANT_NODE, $1, 0); }
             | FLOAT {$$ = newNode(FLOAT_CONSTANT_NODE, NULL, 0); }
             ; 

string_value: STRING {$$ = newNode(STRING_CONSTANT_NODE, NULL, 0); };

boolean_value: BOOLEAN {$$ = newNode(BOOLEAN_CONSTANT_NODE, NULL, 0); };

/* sum: sum PLUS sum {$$ = newNode(PLUS_NODE, NULL, 2, $1, $3); }
   | INTEGER { $$ = newNode(INTEGER_CONSTANT_NODE, NULL, 0); }
   ; */

code_block: /* empty */ { $$ = NULL; }
           //| code_block code_block
           | if {$$ = $1; }
           //| variable_definition
           //| function_call
           ;


if: IF '(' conditional ')' '{' code_block '}'  {$$ = newNode(IF_NODE, NULL, 2, $3, $6)};

conditional: conditional AND conditional {$$ = newNode(AND_NODE, NULL, 2, $1, $3); }
            | conditional OR conditional {$$ = newNode(OR_NODE, NULL, 2, $1, $3); }
            | numeric_expression LT numeric_expression {$$ = newNode(LT_NUMERIC_NODE, NULL, 2, $1, $3); }
            | numeric_expression GT numeric_expression {$$ = newNode(GT_NUMERIC_NODE, NULL, 2, $1, $3); }
            | numeric_expression LE numeric_expression {$$ = newNode(LE_NUMERIC_NODE, NULL, 2, $1, $3); }
            | numeric_expression GE numeric_expression {$$ = newNode(GE_NUMERIC_NODE, NULL, 2, $1, $3); }
            | numeric_expression EQ numeric_expression {$$ = newNode(EQ_NUMERIC_NODE, NULL, 2, $1, $3); }
            | numeric_expression NEQ numeric_expression {$$ = newNode(NEQ_NUMERIC_NODE, NULL, 2, $1, $3); }
            | string_expression EQ string_expression {$$ = newNode(EQ_STRING_NODE, NULL, 2, $1, $3); }
            | string_expression NEQ string_expression {$$ = newNode(NEQ_STRING_NODE, NULL, 2, $1, $3); }
            | boolean_expression EQ boolean_expression {$$ = newNode(EQ_BOOLEAN_NODE, NULL, 2, $1, $3); }
            | boolean_expression NEQ boolean_expression {$$ = newNode(NEQ_BOOLEAN_NODE, NULL, 2, $1, $3); }
            | boolean_expression  { $$ = $1; }
            ;
            
//No soportamos que dentro del if o while haya un numero o un string como condicional, 
//asique si queres hacer un ciclo infinito pones while(true)

numeric_expression: numeric_expression PLUS numeric_expression {$$ = newNode(PLUS_NODE, NULL, 2, $1, $3); }
                    | numeric_expression MINUS numeric_expression {$$ = newNode(MINUS_NODE, NULL, 2, $1, $3); }
                    | numeric_expression TIMES numeric_expression {$$ = newNode(TIMES_NODE, NULL, 2, $1, $3); }
                    | numeric_expression DIVIDE numeric_expression {$$ = newNode(DIVIDE_NODE, NULL, 2, $1, $3); }
                    | numeric_expression MODULE numeric_expression {$$ = newNode(MODULE_NODE, NULL, 2, $1, $3); }
                    | numeric_value { $$ = $1; }
                    ;

boolean_expression: boolean_value { $$ = $1; }
                    ;

string_expression: string_value { $$ = $1; }
                    ;

//while: WHILE '(' conditional ')' '{' code_block '}' {$$ = newNode(WHILE_NODE, NULL, 2, $3, $6)}; 

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