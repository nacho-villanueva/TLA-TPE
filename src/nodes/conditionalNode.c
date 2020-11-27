#include <stdio.h>
#include "conditionalNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

// int parseConditionalNode(Node * node, U3D_Context *  context) {
    
//     int ret = 0;

//     if(node->childrenCount != 1){
//         logError(SYNTAX_ERROR, "Expected 1 child\n");
//         return -1;
//     }

//     if(
//        node -> children[0] -> type != AND_NODE && 
//        node -> children[0] -> type != OR_NODE && 
//        node -> children[0] -> type != LT_NUMERIC_NODE && 
//        node -> children[0] -> type != GT_NUMERIC_NODE && 
//        node -> children[0] -> type != LE_NUMERIC_NODE && 
//        node -> children[0] -> type != EQ_NUMERIC_NODE && 
//        node -> children[0] -> type != NEQ_NUMERIC_NODE && 
//        node -> children[0] -> type != EQ_STRING_NODE && 
//        node -> children[0] -> type != NEQ_STRING_NODE && 
//        node -> children[0] -> type != EQ_BOOLEAN_NODE && 
//        node -> children[0] -> type != NEQ_BOOLEAN_NODE &&
//        node -> children[0] -> type != BOOLEAN_CONSTANT_NODE &&
//        node -> children[0] -> type != INTEGER_CONSTANT_NODE
//        ){
//            printf("----%d-----\n", node->children[0]->type);
//         logError(SYNTAX_ERROR, "Conditional node expected\n");
//         return -1;
//     }

//     ret += parseNode(node->children[0], context);

//     return ret;
// }

int parseOrAndConditionalNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    // if(node -> children[0] -> type != CONDITIONAL_NODE){
    //     logError(SYNTAX_ERROR, "Conditional node expected\n");
    //     return -1;
    // }

    if(
       node -> children[0] -> type != AND_NODE && 
       node -> children[0] -> type != OR_NODE && 
       node -> children[0] -> type != LT_NUMERIC_NODE && 
       node -> children[0] -> type != GT_NUMERIC_NODE && 
       node -> children[0] -> type != LE_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_NUMERIC_NODE && 
       node -> children[0] -> type != NEQ_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_STRING_NODE && 
       node -> children[0] -> type != NEQ_STRING_NODE && 
       node -> children[0] -> type != EQ_BOOLEAN_NODE && 
       node -> children[0] -> type != NEQ_BOOLEAN_NODE &&
       node -> children[0] -> type != BOOLEAN_CONSTANT_NODE
       ){
           printf("----%d-----\n", node->children[0]->type);
        logError(SYNTAX_ERROR, "Some type of conditional node expected\n");
        return -1;
    }

    
    ret += parseNode(node -> children[0], context);

    if(node->type == AND_NODE) {
        parse(" && ");
        //TODO: chequear valor de retorno de la 1era parte. SI es false, no analizamos el 2do nodo
    } else if(node->type == OR_NODE) {
        parse(" || ");
        //TODO: chequear valor de retorno de la 1era parte. SI es true, no analizamos el 2do nodo
    }

    // if(node -> children[1] -> type != CONDITIONAL_NODE){
    //     logError(SYNTAX_ERROR, "Conditional node expected\n");
    //     return -1;
    // }

    if(
       node -> children[0] -> type != AND_NODE && 
       node -> children[0] -> type != OR_NODE && 
       node -> children[0] -> type != LT_NUMERIC_NODE && 
       node -> children[0] -> type != GT_NUMERIC_NODE && 
       node -> children[0] -> type != LE_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_NUMERIC_NODE && 
       node -> children[0] -> type != NEQ_NUMERIC_NODE && 
       node -> children[0] -> type != EQ_STRING_NODE && 
       node -> children[0] -> type != NEQ_STRING_NODE && 
       node -> children[0] -> type != EQ_BOOLEAN_NODE && 
       node -> children[0] -> type != NEQ_BOOLEAN_NODE &&
       node -> children[0] -> type != BOOLEAN_CONSTANT_NODE
       ){
           printf("----%d-----\n", node->children[0]->type);
        logError(SYNTAX_ERROR, "Some type of conditional node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1], context);

    return ret;
}

int parseNumericConditionalNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    for(int i = 0; i < node->childrenCount; i++) {
        if(node->children[i]->type != INTEGER_CONSTANT_NODE && node->children[i]->type != DOUBLE_CONSTANT_NODE 
                && node->children[i]->type != PLUS_NODE && node->children[i]->type != MINUS_NODE && node->children[i]->type != TIMES_NODE 
                && node->children[i]->type != DIVIDE_NODE && node->children[i]->type != MODULE_NODE) {
            logError(SYNTAX_ERROR, "Numeric expression node expected (in conditionalNode)\n");
            return -1;
        }
    }

    ret += parseNode(node -> children[0], context);

    switch(node->type){
        case LT_NUMERIC_NODE: parse(" < "); break;
        case GT_NUMERIC_NODE: parse(" > "); break;
        case LE_NUMERIC_NODE: parse(" <= "); break;
        case GE_NUMERIC_NODE: parse(" >= "); break;
        case EQ_NUMERIC_NODE: parse(" == "); break;
        case NEQ_NUMERIC_NODE: parse(" != "); break;
        default: 
            // TODO: revisar este caso
            logError(FATAL_ERROR, "Impossible conditionalNode state. Returning -1\n");
            return -1;
    }

    ret += parseNode(node -> children[1], context);

    return ret;
}

int parseStringConditionalNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    if(node -> children[0] -> type != STRING_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "String constant node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0], context);

    if(node->type == EQ_STRING_NODE) {
        parse(" == ");
    } else if(node->type == NEQ_STRING_NODE) {
        parse(" != ");
    }
    
    if(node -> children[1] -> type != STRING_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "String constant node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1], context);

    return ret;
}

int parseBooleanConditionalNode(Node * node, U3D_Context *  context) {
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "Expected 2 children\n");
        return -1;
    }

    if(node -> children[0] -> type != BOOLEAN_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Boolean constant node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0], context);

    if(node->type == EQ_BOOLEAN_NODE) {
        parse(" == ");
    } else if(node->type == NEQ_BOOLEAN_NODE) {
        parse(" != ");
    }
    
    if(node -> children[1] -> type != BOOLEAN_CONSTANT_NODE){
        logError(SYNTAX_ERROR, "Boolean constant node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1], context);

    return ret;
}
