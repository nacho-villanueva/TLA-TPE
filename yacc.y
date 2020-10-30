 %{
    #include <stdio.h>

    void yyerror();
    int yylex();
    
    extern int yylineno;

 %}

%union {
    int number;
    char * string;
    char character;
    float decimal;
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

%type<number> sum

%right EQUAL
%left PLUS MINUS
%left TIMES DIVIDE


%start start;

 %%

start: start_program;

start_program: START_PROGRAM ENDL {printf("public class Main() {\n\tpublic static void main () {\n\t\t");} begin;

begin: line | begin line;

line: define_figure | sum ENDL | end_program;

end_program: END_PROGRAM ENDL  {printf("\t}\n}");};

define_figure: FIGURE_TYPE IDENTIFIER EQUAL BRACKET_OPEN figure_atributes BRACKET_CLOSE {printf("\t--Created figure: '%s'--\n", $2);};

figure_atributes: figure_atributes figure_atribute
                | figure_atribute;

figure_atribute: IDENTIFIER COLON value ENDL {printf("\tAttr Name: %s\n", $1);};

value: INTEGER {printf("Attr Value: (int)%d\t--", $1);}
    | FLOAT {printf("Attr Value: (float)%f\t--", $1);}
    | STRING {printf("Attr Value: (string)%s\t--", $1);};

sum: sum PLUS sum {$$ = $1 + $3; printf("Result= %d\n",$$);}
    | INTEGER {$$ = $1; printf("+ %d\n", $1); }
    ; 

 %%

int yywrap()
{
        return 1;
} 

int main() {
    yyparse();
} 