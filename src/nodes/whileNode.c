#include "whileNode.h"
#include "../utils/logger.h"

int parseWhileNode(Node * node){
    int ret = 0;

    if(node->childrenCount != 2){
        logError(SYNTAX_ERROR, "While node expects 2 children\n");
        return -1;
    }
    parse("while(");

    if(node -> children[0] -> type != CONDITIONAL_NODE){
        logError(SYNTAX_ERROR, "Conditional node expected\n");
        return -1;
    }
    ret += parseNode(node -> children[0]);

    parse(") {\n\t");
    if(node -> children[1] -> type != CODE_BLOCK_NODE){
        logError(SYNTAX_ERROR, "Code block noded expected\n");
        return -1;
    }
    ret += parseNode(node -> children[1]);
    parse("}\n");

    return ret;
}