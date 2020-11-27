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

    union NodeValue emptyNodeValue;
%}

%parse-param {struct Node * root }

%union {
    int number;
    char * string;
    char character;
    double decimal;
    //float decimal;
    struct Node * node;
    bool boolean;
}

%token SETTINGS_BLOCK DRAW_BLOCK END_BLOCK 
%token<character> ENDL
%token<character> EQUAL COLON
%token<character> PLUS MINUS TIMES DIVIDE MODULE
%token<string> INT_TYPE FLOAT_TYPE DOUBLE_TYPE FIGURE_TYPE FUNCTION_TYPE BOOLEAN_TYPE
%token<string> IDENTIFIER
%token<number> INTEGER
%token<decimal> DOUBLE
%token<string> STRING
%token<boolean> BOOLEAN
%token<character> BRACKET_OPEN BRACKET_CLOSE OPEN CLOSE
%token WHILE
%token IF
%token AND OR GT LT GE LE EQ NEQ

%type<node> declaration_list definition
%type<node> define_figure
%type<node> figure_atributes
%type<node> figure_atribute
%type<node> value
%type<node> block_list
%type<node> block
%type<node> numeric_value
%type<node> string_value
%type<node> boolean_value
%type<node> numeric_expression
%type<node> code_block
%type<node> code_line
%type<node> if
%type<node> while
%type<node> conditional



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
     | DRAW_BLOCK code_block END_BLOCK { $$ = newNode(DRAW_NODE, emptyNodeValue, 1, $2); }
     ; 

declaration_list: declaration_list definition { addChildrenToNode($1, 1, $2); }
               | definition { $$ = newNode(DEFINITIONS_NODE, emptyNodeValue, 1, $1); }
               ;

definition: define_figure { $$ = newNode(VARIABLE_NODE, emptyNodeValue, 1, $1); };

define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atributes BRACKET_CLOSE {$$ = newNode(FIGURE_NODE, emptyNodeValue, 1, $5);};

figure_atributes: figure_atributes figure_atribute { addChildrenToNode($1, 1, $2); }
                | figure_atribute {$$ = newNode(ATTRIBUTE_LIST_NODE, emptyNodeValue, 1, $1); }
                ;

figure_atribute: IDENTIFIER COLON value ENDL { $$ = newNode(ATTRIBUTE_NODE, emptyNodeValue, 1, $3); };

value: numeric_value { $$ = $1; }
     | string_value { $$ = $1; }
     | boolean_value { $$ = $1; }
     ;

numeric_value: INTEGER { union NodeValue integer_constant_value;
                         integer_constant_value.integer = $1;
                         $$ = newNode(INTEGER_CONSTANT_NODE,  integer_constant_value, 0); 
             }
             | DOUBLE {union NodeValue double_constant_value;
                         double_constant_value.decimal = $1;
                         $$ = newNode(DOUBLE_CONSTANT_NODE,  double_constant_value, 0);
             }
             ; 

string_value: STRING {union NodeValue string_constant_value;
                      string_constant_value.string = $1;
                      $$ = newNode(STRING_CONSTANT_NODE,  string_constant_value, 0);
            };


boolean_value: BOOLEAN { union NodeValue boolean_constant_value; 
                         boolean_constant_value.boolean = $1;
                         $$ = newNode(BOOLEAN_CONSTANT_NODE, boolean_constant_value, 0); };

// Comentario: code_block no puede ser vacío (daba shift/reduce conflict)
code_block: code_block code_line { addChildrenToNode($1, 1, $2); }
           | code_line {$$ = newNode(CODE_BLOCK_NODE, emptyNodeValue, 1, $1); }
           ;
           

code_line: if { $$ = newNode(CODE_LINE_NODE, emptyNodeValue, 1, $1); }
           | numeric_value { $$ = newNode(CODE_LINE_NODE, emptyNodeValue, 1, $1); }
           | while { $$ = newNode(CODE_LINE_NODE, emptyNodeValue, 1, $1); }
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