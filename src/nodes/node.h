#ifndef _NODE_H_
#define _NODE_H_

#include "../u3d_context.h"
#include <stdbool.h>
#define NODE_SIZE sizeof(struct Node)

static const char * NODE_NAMES[] = {
    "ROOT_NODE",
    "DEFINITIONS_NODE",
    "SETTINGS_NODE",
    "DRAW_NODE",

    "PLUS_NODE",
    "MINUS_NODE",
    "TIMES_NODE",
    "DIVIDE_NODE",
    "MODULE_NODE",

    "IDENTIFIER_NODE",
    "VALUE_NODE",
    "VARIABLE_NODE",

    "INTEGER_CONSTANT_NODE",
    "STRING_CONSTANT_NODE",
    "FLOAT_CONSTANT_NODE",
    "BOOLEAN_CONSTANT_NODE",
    "VECTOR3_NODE",
    "VECTOR3INT_NODE",

    "FIGURE_NODE",
    "FIGURE_ATTRIBUTE_LIST_NODE",
    "FIGURE_ATTRIBUTE_NODE",

    /* conditional names */
    //"CONDITIONAL_NODE",
    "AND_NODE",
    "OR_NODE",
    /* numeric expression names */
    "LT_NUMERIC_NODE",
    "GT_NUMERIC_NODE",
    "LE_NUMERIC_NODE",
    "GE_NUMERIC_NODE",
    "EQ_NUMERIC_NODE",
    "NEQ_NUMERIC_NODE",
    /* string expression names */
    "EQ_STRING_NODE",
    "NEQ_STRING_NODE",
     /* boolean expression names */
    "EQ_BOOLEAN_NODE",
    "NEQ_BOOLEAN_NODE",
    /* double identifier expression names */
    "LT_IDENTIFIER_NODE",
    "GT_IDENTIFIER_NODE",
    "LE_IDENTIFIER_NODE",
    "GE_IDENTIFIER_NODE",
    "EQ_IDENTIFIER_NODE",
    "NEQ_IDENTIFIER_NODE",
    
    "WHILE_NODE",
    "IF_NODE",

    "CODE_BLOCK_NODE",

    "FUNCTION_IDENTIFIER_NODE",
    "FUNCTION_CALL_NODE",
    "PARAMETERS_LIST",

    /*variable types*/
    "STRING_VARIABLE_CREATION_NODE",
    "INTEGER_VARIABLE_CREATION_NODE",
    "FLOAT_VARIABLE_CREATION_NODE",
    "BOOLEAN_VARIABLE_CREATION_NODE",

    /* variable update types */
    "NUMERIC_VARIABLE_UPDATE_NODE",
    "STRING_VARIABLE_UDATE_NODE",
    "BOOLEAN_VARIABLE_UPDATE_NODE",
    "IDENTIFIER_VARIABLE_UPDATE_NODE"
};

typedef enum nodeType{
    ROOT_NODE = 0,
    DEFINITIONS_NODE,
    SETTINGS_NODE,
    DRAW_NODE,

    PLUS_NODE,   /* x + y || 5 + y */
    MINUS_NODE,  /* x - y || 5 - y */
    TIMES_NODE,  /* x * y || 5 * y */
    DIVIDE_NODE, /* x / y || 5 / y */
    MODULE_NODE, /* x % y || 5 % y */

    IDENTIFIER_NODE,
    VALUE_NODE,
    VARIABLE_NODE,

    INTEGER_CONSTANT_NODE,  /* 5 */
    STRING_CONSTANT_NODE,   /* "hola" */
    FLOAT_CONSTANT_NODE,    /* 5.5 */
    BOOLEAN_CONSTANT_NODE,  /* true */
    VECTOR3_NODE,           /* (1.0, 2.0, 3.0) */
    VECTOR3INT_NODE,        /* (1, 2, 3) */

    FIGURE_NODE,
    FIGURE_ATTRIBUTE_LIST_NODE,
    FIGURE_ATTRIBUTE_NODE,
    /* conditional names */
    AND_NODE,   /* A && B */
    OR_NODE,    /* A || B */

    /* numeric expression names */
    LT_NUMERIC_NODE,  /* x < y   ||  5 < y  */
    GT_NUMERIC_NODE,  /* x > y   ||  5 > y  */
    LE_NUMERIC_NODE,  /* x <= y  ||  5 <= y */
    GE_NUMERIC_NODE,  /* x >= y  ||  5 >= y */
    EQ_NUMERIC_NODE,  /* x == y  ||  5 == y */
    NEQ_NUMERIC_NODE, /* x != y  ||  5 != y */

    /* string expression names */
    EQ_STRING_NODE,   /* x == y  ||  "hola" == y */
    NEQ_STRING_NODE,  /* x != y  ||  "hola" != y */

    /* boolean expression names */
    EQ_BOOLEAN_NODE,  /* x == y  ||  true == y */
    NEQ_BOOLEAN_NODE, /* x != y  ||  true != y */

    /* double identifier expression names */
    LT_IDENTIFIER_NODE,  /* x < y */
    GT_IDENTIFIER_NODE,  /* x > y */
    LE_IDENTIFIER_NODE,  /* x <= y */
    GE_IDENTIFIER_NODE,  /* x >= y */
    EQ_IDENTIFIER_NODE,  /* x == y */
    NEQ_IDENTIFIER_NODE, /* x != y */

    WHILE_NODE,       /* while(A){B} */
    IF_NODE,          /* if(A){B} */
    
    CODE_BLOCK_NODE,

    FUNCTION_IDENTIFIER_NODE,
    FUNCTION_CALL_NODE,
    PARAMETERS_LIST,

    /* variable creation types */
    STRING_VARIABLE_CREATION_NODE,  /* string a = "hola"; || string a; */
    INTEGER_VARIABLE_CREATION_NODE, /* int a = 4;         || int a; */
    FLOAT_VARIABLE_CREATION_NODE,   /* float a = 4.5;     || float a; */
    BOOLEAN_VARIABLE_CREATION_NODE, /* boolean a = trueM  || boolean a; */

    /* variable update types */
    NUMERIC_VARIABLE_UPDATE_NODE,    /* a = 5  ||   a = 5.5 */
    STRING_VARIABLE_UPDATE_NODE,     /* a = "hola" */
    BOOLEAN_VARIABLE_UPDATE_NODE,    /* a = true */
    IDENTIFIER_VARIABLE_UPDATE_NODE, /* a = b */

} NodeType;

typedef union NodeValue {
    int integer;
    float decimal;
    char * string;
    bool boolean;
    Vector3 vector;
    Vector3Int vectorInt;
} NodeValue;

typedef struct Node{
    NodeType type;
    NodeValue value;
    int childrenCount;
    struct Node ** children;
} Node;

/*
Generate Node.
*/
Node* newNode(NodeType type, NodeValue value, int childrenCount, ...);

/*
Append multiple children node to another node.
*/
void addChildrenToNode(Node* node, int childrenCount, ...);

/*
Parse node.
Returns -1 on error.
*/
int parseNode(Node* node, U3D_Context *  context);

Node * getChildNode(Node * node, NodeType type);

int castNode(Node * node, NodeType toType);

const char * getNodeTypeByCode(NodeType type);

/*
Print node tree to stdout.
*/
void printTree(Node * node);

#endif //NODE_H