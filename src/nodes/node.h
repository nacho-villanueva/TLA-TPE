#ifndef _NODE_H_
#define _NODE_H_

#include "../u3ddef.h"
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
    "FIGURE_NODE",
    "ATTRIBUTE_LIST_NODE",
    "ATTRIBUTE_NODE",
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
    "WHILE_NODE",
    "IF_NODE",
    "CODE_BLOCK_NODE",
    "CODE_LINE_NODE",
};

typedef enum nodeType{
    ROOT_NODE = 0,
    DEFINITIONS_NODE,
    SETTINGS_NODE,
    DRAW_NODE,
    PLUS_NODE,
    MINUS_NODE,
    TIMES_NODE,
    DIVIDE_NODE,
    MODULE_NODE,
    IDENTIFIER_NODE,
    VALUE_NODE,
    VARIABLE_NODE,
    INTEGER_CONSTANT_NODE,
    STRING_CONSTANT_NODE,
    FLOAT_CONSTANT_NODE,
    BOOLEAN_CONSTANT_NODE,
    FIGURE_NODE,
    FIGURE_ATTRIBUTE_LIST_NODE,
    FIGURE_ATTRIBUTE_NODE,
    /* conditional names */
    //CONDITIONAL_NODE,
    AND_NODE,
    OR_NODE,
    /* numeric expression names */
    LT_NUMERIC_NODE,
    GT_NUMERIC_NODE,
    LE_NUMERIC_NODE,
    GE_NUMERIC_NODE,
    EQ_NUMERIC_NODE,
    NEQ_NUMERIC_NODE,
    /* string expression names */
    EQ_STRING_NODE,
    NEQ_STRING_NODE,
    /* boolean expression names */
    EQ_BOOLEAN_NODE,
    NEQ_BOOLEAN_NODE,
    WHILE_NODE,
    IF_NODE,
    CODE_BLOCK_NODE,
    CODE_LINE_NODE,
} NodeType;

typedef union NodeValue {
    int integer;
    float floating;
    char * string;
    bool boolean;
} NodeValue;

typedef struct Node
{
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

/*
Print node tree to stdout.
*/
void printTree(Node * node);

#endif //NODE_H