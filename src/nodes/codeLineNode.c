#include <stdio.h>  // TODO: quizas se borre
#include "codeLineNode.h"
#include "../utils/logger.h"
#include "../utils/parser.h"

int parseCodeLineNode(Node * node, U3D_Context *  context) {
    int ret = 0;
    // if(node->childrenCount != 1){
    //     logError(SYNTAX_ERROR, "Expected 1 child\n");
    //     return -1;
    // }

    if(node->childrenCount == 0) {
        printf("CodeLineBlock tiene cero hijos.\n");
        return 0;
    }

    switch(node->children[0]->type){
        case IF_NODE:
            printf("CodeLineBlock va a llamar al parseNode(IF_NODE)\n");
            return parseNode(node->children[0], context);
        case INTEGER_CONSTANT_NODE:
            printf("parseando LINE_NODE. Hijo es de tipo INTEGER_CONSTANT_NODE\n"); // TODO: borrar
            parse("%d",node->children[0]->value.integer);
            return 0;
        default:
            logError(SYNTAX_ERROR, "Impossible children type in CODE_LINE_NODE\n");
            return -1;
    }

    return ret;
}
