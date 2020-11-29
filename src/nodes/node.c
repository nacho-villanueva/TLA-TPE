#include "node.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../utils/logger.h"
#include "../utils/parser.h"

#include "conditionalNode.h"
#include "codeBlockNode.h"
#include "codeLineNode.h"
#include "numericExpressionNode.h"
#include "ifWhileNode.h"
#include "rootNodes.h"
#include "figureNode.h"
#include "functionCallNode.h"

Node* newNode(NodeType type, NodeValue value, int childrenCount, ...) {
    Node* node = malloc(NODE_SIZE);
    logDebug("Creating node %p (NodeType: %s)\n", node, NODE_NAMES[type]);
    node->type = type;
    node->value = value;
    node->childrenCount=childrenCount;
    node->children = malloc(NODE_SIZE * childrenCount);

    va_list valist;
    va_start(valist, childrenCount);
    for (int i = 0; i < childrenCount; i++)
        node->children[i] = va_arg(valist, Node*);
    va_end(valist);

    return node;
}

void addChildrenToNode(Node* node, int newChildrenCount, ...){
    node->children = realloc(node->children, NODE_SIZE * node->childrenCount + newChildrenCount);
    
    va_list valist;
    va_start(valist, newChildrenCount);
    for (int i = 0; i < newChildrenCount; i++){
        node->children[node->childrenCount + i] = va_arg(valist, Node*);
        logDebug("Adding child node %p (NodeType: %s) to node %p (NodeType: %s)\n", node->children[node->childrenCount + i], node->children[node->childrenCount + i] != NULL ? NODE_NAMES[node->children[node->childrenCount + i]->type] : "NULL", node, NODE_NAMES[node->type]);
    }
    va_end(valist);
    node->childrenCount += newChildrenCount;
}

int parseNode(Node* node, U3D_Context * context){
    if(node == NULL){
        logInfo("WARNING: parseNode called with NULL node.\n");
        return 0;
    }
    switch (node->type)
    {
    case ROOT_NODE:
        return parseRootNode(node, context);
    case DEFINITIONS_NODE:
        return parseDefinitionsNode(node, context);
    case SETTINGS_NODE:
        return parseSettingsNode(node, context);
    case DRAW_NODE:
        return parseDrawNode(node, context);
    case FIGURE_NODE:
       return parseFigureNode(node, context);
    case IF_NODE:
    case WHILE_NODE:
        return parseIfWhileNode(node, context);
    case AND_NODE:
    case OR_NODE:
        return parseOrAndConditionalNode(node, context);
    case LT_NUMERIC_NODE:
    case GT_NUMERIC_NODE:
    case LE_NUMERIC_NODE:
    case GE_NUMERIC_NODE:
    case EQ_NUMERIC_NODE:
    case NEQ_NUMERIC_NODE:
        return parseNumericConditionalNode(node, context);
    case EQ_STRING_NODE:
    case NEQ_STRING_NODE:
        return parseStringConditionalNode(node, context);
    case EQ_BOOLEAN_NODE:
    case NEQ_BOOLEAN_NODE:
        return parseBooleanConditionalNode(node, context);
    case PLUS_NODE:
    case MINUS_NODE:
    case TIMES_NODE:
    case DIVIDE_NODE:
    case MODULE_NODE:
        return parseNumericExpressionNode(node, context);
    case BOOLEAN_CONSTANT_NODE:
        if(node->value.boolean == true)
            parse("%s", "true");
        else
            parse("%s", "false");
        return 0;
    case INTEGER_CONSTANT_NODE:
        parse("%d", node->value.integer);
        return 0;
    case FLOAT_CONSTANT_NODE:
        parse("%f", node->value.decimal);
        return 0;
    case STRING_CONSTANT_NODE:
        parse("%s", node->value.string);
        return 0;
    case CODE_BLOCK_NODE:
        return parseCodeBlockNode(node, context);
    case CODE_LINE_NODE:
        return parseCodeLineNode(node, context);
    case FUNCTION_CALL_NODE:
        return parseFunctionCallNode(node, context);
    default:
        logInfo("WARNING: Node parser not assigned (Type: %s)\n", NODE_NAMES[node->type]);
        return -1;
    }
}

void printNodeValue(Node * node){
    switch (node->type) {
        case STRING_CONSTANT_NODE:
        case IDENTIFIER_NODE:
            logInfo("(%s)", node->value.string);
            break;
        case INTEGER_CONSTANT_NODE:
            logInfo("(%d)", node->value.integer);
            break;
        case FLOAT_CONSTANT_NODE:
            logInfo("(%f)", node->value.decimal);
            break;
        case BOOLEAN_CONSTANT_NODE:
            logInfo("(%d)", node->value.boolean);
            break;
        case VECTOR3_NODE:
            printVector3(node->value.vector);
            break;
        case VECTOR3INT_NODE:
            printVector3Int(node->value.vectorInt);
            break;
        default:
            break;
    }
}

void printTreeRec(Node * node, int step){
    for(int i = 0; i < step; i++){
        logInfo("   ");
    }
    if(node != NULL) {
        logInfo("|%s ", NODE_NAMES[node->type]);
        printNodeValue(node);
        logInfo("\n");
    }
    if(node->childrenCount > 0){
        for(int i = 0; i < step; i++){
            logInfo("   ");
        }
        logInfo("   \\________\n");
        for(int i = 0; i < node->childrenCount; i++){
            if(node->children[i])
                printTreeRec(node->children[i], step+1);
        }
    }
}

int castNode(Node * node, NodeType toType){
    if(node->type == toType)
        return 0;
    switch (toType) {
        case VECTOR3_NODE:
            if(node->type == VECTOR3INT_NODE){
                node->type = VECTOR3_NODE;
                node->value.vector = vector3IntToVector3(node->value.vectorInt);
                return 0;
            }
            break;
        case VECTOR3INT_NODE:
            if(node->type == VECTOR3_NODE){
                node->type = VECTOR3INT_NODE;
                node->value.vectorInt = vector3ToVector3Int(node->value.vector);
                logWarning("Implicit cast of Vector3Int to Vector3\n");
                return 0;
            }
            break;
        case FLOAT_CONSTANT_NODE:
            if(node->type == INTEGER_CONSTANT_NODE){
                node->type = FLOAT_CONSTANT_NODE;
                node->value.decimal = (float)node->value.integer;
                return 0;
            }
            break;
        case INTEGER_CONSTANT_NODE:
            if(node->type == FLOAT_CONSTANT_NODE){
                node->type = INTEGER_CONSTANT_NODE;
                node->value.integer = (int)node->value.decimal;
                return 0;
            } else if(node->type == BOOLEAN_CONSTANT_NODE){
                node->type = INTEGER_CONSTANT_NODE;
                node->value.integer = node->value.boolean ? 1 : 0;
                return 0;
            }
        default:
            break;
    }

    return -1;
}

void printTree(Node * node) {
    printTreeRec(node, 0);
}

Node *getChildNode(Node *node, NodeType type) {
    Node * ret = NULL;
    for(int i = 0; i < node->childrenCount; i++){
        if(node->children[i]->type == type) {
            if (ret == NULL)
                ret = node->children[i];
            else
                logInfo("WARNING: getChildNode(): Found multiple children of type: %s. Returning the first.",
                        NODE_NAMES[type]);
        }
    }
    return ret;
}

