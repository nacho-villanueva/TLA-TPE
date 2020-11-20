#ifndef _NODE_H_
#define _NODE_H_

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
    "VARIABLE_NODE",
    "INTEGER_CONSTANT_NODE",
    "STRING_CONSTANT_NODE",
    "FLOAT_CONSTANT_NODE",
    "FIGURE_NODE",
    "ATTRIBUTE_LIST_NODE",
    "ATTRIBUTE_NODE",
};

typedef enum _nodeType{
    ROOT_NODE = 0,
    DEFINITIONS_NODE,
    SETTINGS_NODE,
    DRAW_NODE,
    PLUS_NODE,
    MINUS_NODE,
    TIMES_NODE,
    DIVIDE_NODE,
    VARIABLE_NODE,
    INTEGER_CONSTANT_NODE,
    STRING_CONSTANT_NODE,
    FLOAT_CONSTANT_NODE,
    FIGURE_NODE,
    ATTRIBUTE_LIST_NODE,
    ATTRIBUTE_NODE,
} NodeType;

typedef void * NodeValue;

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
int parseNode(Node* node);

/*
Print node tree to stdout.
*/
void printTree(Node * node);

#endif //NODE_H