#include <stdio.h>
#include "ifWhileNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseIfWhileNode(Node * node, U3D_Context *  context){
    int ret = 0;
    
    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "If node expects 2 children\n");
        return -1;
    }
    
    if(node->type == IF_NODE){
        parse("if(");
    }else if(node->type == WHILE_NODE){
        parse("while(");
    }

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
       node -> children[0] -> type != BOOLEAN_CONSTANT_NODE &&
       node -> children[0] -> type != INTEGER_CONSTANT_NODE &&
       node -> children[0] -> type != DOUBLE_CONSTANT_NODE
       ){
           printf("----%d-----\n", node->children[0]->type);
        logError(SYNTAX_ERROR, "Some type of conditional node expected\n");
        return -1;
    }

    parseNode(node->children[0], context);

    parse(") {\n\t");

    if(node -> children[1] -> type != CODE_BLOCK_NODE){
        logError(SYNTAX_ERROR, "Code block node expected\n");
        return -1;
    }

    ret += parseNode(node -> children[1], context);

    parse("}\n");

    return ret;
}