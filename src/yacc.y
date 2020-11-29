%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stddef.h>
    #include <stdbool.h>
    #include <strings.h>

    #include "src/nodes/node.h"
    #include "src/utils/logger.h"
    #include "src/u3d.h"
    #include "src/adt/vector3.h"

    bool hasError = false;
    void yyerror (struct Node * node, char const * msg);
    int yylex();

    extern FILE *yyin;
    extern int yylineno;

    union NodeValue emptyNodeValue;
%}

%parse-param {struct Node * root }

%union {
    int number;
    char * string;
    char character;
    //double decimal;
    float decimal;
    struct Node * node;
    bool boolean;
}

%token SETTINGS_BLOCK DRAW_BLOCK END_BLOCK 
%token<character> ENDL
%token<character> EQUAL COLON
%token<character> PLUS MINUS TIMES DIVIDE MODULE
%token<string> INT_TYPE FLOAT_TYPE FIGURE_TYPE FUNCTION_TYPE BOOLEAN_TYPE
%token<string> IDENTIFIER
%token<number> INTEGER
%token<decimal> FLOAT
%token<string> STRING
%token<boolean> BOOLEAN
%token<character> BRACKET_OPEN BRACKET_CLOSE OPEN CLOSE
%token COMMA DOT
%token WHILE
%token IF
%token AND OR GT LT GE LE EQ NEQ

%type<node> draw
%type<node> declaration_list definition

%type<node> define_figure
%type<node> figure_atribute_list
%type<node> figure_atribute

%type<node> function_identifier function_call
%type<node> parameters_list

%type<node> value
%type<node> block_list
%type<node> block
%type<node> vector_value
%type<node> numeric_value
%type<node> string_value
%type<node> boolean_value
%type<node> numeric_expression
%type<node> code_block
%type<node> code_line
%type<node> if
%type<node> while
%type<node> conditional
%type<node> identifier


%left AND
%left OR
%left PLUS MINUS
%left TIMES DIVIDE 
%left MODULE
//%left GT LT GE LE EQ NEQ

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

block: SETTINGS_BLOCK END_BLOCK { $$ = newNode(SETTINGS_NODE, emptyNodeValue, 0); }
     | draw { $$ = $1; }
     ;

draw: DRAW_BLOCK END_BLOCK { $$ = newNode(DRAW_NODE, emptyNodeValue, 0); }
    | DRAW_BLOCK code_block END_BLOCK { $$ = newNode(DRAW_NODE, emptyNodeValue, 1, $2); }


declaration_list: declaration_list definition { addChildrenToNode($1, 1, $2); }
               | definition { $$ = newNode(DEFINITIONS_NODE, emptyNodeValue, 1, $1); }
               ;

definition: define_figure { $$ = $1; }

define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atribute_list BRACKET_CLOSE {$$ = newNode(FIGURE_NODE, (NodeValue)$2, 1, $5); }
	         ;

figure_atribute_list: figure_atribute_list figure_atribute { addChildrenToNode($1, 1, $2); }
                | figure_atribute {$$ = newNode(FIGURE_ATTRIBUTE_LIST_NODE, emptyNodeValue, 1, $1); }
                ;

figure_atribute: identifier COLON value ENDL { $$ = newNode(FIGURE_ATTRIBUTE_NODE, emptyNodeValue, 2, $1, $3); }
               ;

identifier: IDENTIFIER { $$ = newNode(IDENTIFIER_NODE, (NodeValue)$1, 0); }

value: numeric_value { $$ = newNode(VALUE_NODE, emptyNodeValue, 1, $1); }
     | string_value { $$ = newNode(VALUE_NODE, emptyNodeValue, 1, $1); }
     | boolean_value { $$ = newNode(VALUE_NODE, emptyNodeValue, 1, $1); }
     | vector_value { $$ = newNode(VALUE_NODE, emptyNodeValue, 1, $1); }
     | identifier{ $$ = newNode(VALUE_NODE, emptyNodeValue, 1, $1); }
     ;


vector_value: OPEN FLOAT COMMA FLOAT COMMA FLOAT CLOSE { $$ = newNode(VECTOR3_NODE, (NodeValue)newVector3($2,$4,$6), 0); }
            | OPEN INTEGER COMMA INTEGER COMMA INTEGER CLOSE { $$ = newNode(VECTOR3INT_NODE, (NodeValue)newVector3Int($2,$4,$6), 0); }

// TODO: ALLOW (1, 1.2, 3)

numeric_value: INTEGER {$$ = newNode(INTEGER_CONSTANT_NODE,  (NodeValue)$1, 0); }
             | FLOAT {$$ = newNode(FLOAT_CONSTANT_NODE, (NodeValue)$1, 0); }
             ; 

string_value: STRING {$$ = newNode(STRING_CONSTANT_NODE, (NodeValue)$1, 0); };


boolean_value: BOOLEAN { $$ = newNode(BOOLEAN_CONSTANT_NODE, (NodeValue)$1, 0); };

// Comentario: code_block no puede ser vacío (daba shift/reduce conflict)
code_block: code_block code_line { addChildrenToNode($1, 1, $2); }
           | code_line {$$ = newNode(CODE_BLOCK_NODE, emptyNodeValue, 1, $1); }
           ;
           

code_line: if { $$ = $1; }
           | while { $$ = $1; }
           | numeric_value { $$ = $1; }
           | function_call { $$ = $1; }
           ;


if: IF OPEN conditional CLOSE BRACKET_OPEN code_block BRACKET_CLOSE  {$$ = newNode(IF_NODE, emptyNodeValue, 2, $3, $6); };

while: WHILE OPEN conditional CLOSE BRACKET_OPEN code_block BRACKET_CLOSE {$$ = newNode(WHILE_NODE, emptyNodeValue, 2, $3, $6); };

conditional: conditional AND conditional {$$ = newNode(AND_NODE, emptyNodeValue, 2, $1, $3); }
            | conditional OR conditional {$$ = newNode(OR_NODE, emptyNodeValue, 2, $1, $3); }
            | numeric_expression LT numeric_expression {$$ = newNode(LT_NUMERIC_NODE, emptyNodeValue, 2, $1, $3); }
            | numeric_expression GT numeric_expression {$$ = newNode(GT_NUMERIC_NODE, emptyNodeValue, 2, $1, $3); }
            | numeric_expression LE numeric_expression {$$ = newNode(LE_NUMERIC_NODE, emptyNodeValue, 2, $1, $3); }
            | numeric_expression GE numeric_expression {$$ = newNode(GE_NUMERIC_NODE, emptyNodeValue, 2, $1, $3); }
            | numeric_expression EQ numeric_expression {$$ = newNode(EQ_NUMERIC_NODE, emptyNodeValue, 2, $1, $3); }
            | numeric_expression NEQ numeric_expression {$$ = newNode(NEQ_NUMERIC_NODE, emptyNodeValue, 2, $1, $3); }
            | string_value EQ string_value {$$ = newNode(EQ_STRING_NODE, emptyNodeValue, 2, $1, $3); }
            | string_value NEQ string_value {$$ = newNode(NEQ_STRING_NODE, emptyNodeValue, 2, $1, $3); }
            
            /*   NO ANDAN LOS TIPOS BOOLEANOS
            | boolean_value EQ boolean_value {$$ = newNode(EQ_BOOLEAN_NODE, emptyNodeValue, 2, $1, $3); }
            | boolean_value NEQ boolean_value {$$ = newNode(NEQ_BOOLEAN_NODE, emptyNodeValue, 2, $1, $3); }
            | boolean_value
            */
            | numeric_value { $$ = $1; }
            ;

numeric_expression: numeric_expression PLUS numeric_expression {$$ = newNode(PLUS_NODE, emptyNodeValue, 2, $1, $3); }
                    | numeric_expression MINUS numeric_expression {$$ = newNode(MINUS_NODE, emptyNodeValue, 2, $1, $3); }
                    | numeric_expression TIMES numeric_expression {$$ = newNode(TIMES_NODE, emptyNodeValue, 2, $1, $3); }
                    | numeric_expression DIVIDE numeric_expression {$$ = newNode(DIVIDE_NODE, emptyNodeValue, 2, $1, $3); }
                    | numeric_expression MODULE numeric_expression {$$ = newNode(MODULE_NODE, emptyNodeValue, 2, $1, $3); }
                    | numeric_value { $$ = $1; }
                    ; 

function_identifier: IDENTIFIER  {$$ = newNode(FUNCTION_IDENTIFIER_NODE, (NodeValue)$1, 0); }
                   ;

/*function_identifier: function_identifier DOT identifier { addChildrenToNode($1, 1, $3); }
                   | identifier { $$ = newNode(FUNCTION_IDENTIFIER_NODE, emptyNodeValue, 1, $1); }
                   ;    
TODO: ALLOW FUNCTION CALL FROM CHILD                   */

function_call: function_identifier OPEN CLOSE ENDL{ $$ = newNode(FUNCTION_CALL_NODE, emptyNodeValue, 1, $1); }
             | function_identifier OPEN parameters_list CLOSE ENDL{ $$ = newNode(FUNCTION_CALL_NODE, emptyNodeValue, 2, $1, $3); }
             ;

parameters_list: parameters_list COMMA value { addChildrenToNode($1, 1, $3); }
               | value {$$ = newNode(PARAMETERS_LIST, emptyNodeValue, 1, $1); }
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
    
    Node * root = newNode(ROOT_NODE, emptyNodeValue, 0, NULL);
    yyparse(root);

    if(!hasError){
        compileU3D(u3d, root);
    }

    printTree(root);

    closeU3D(u3d);
} 