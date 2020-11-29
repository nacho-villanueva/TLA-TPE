#include "codeBlockNode.h"
#include <stdio.h>
#include "../utils/parser.h"
#include "../utils/logger.h"

int parseCodeBlockNode(Node * node, U3D_Context *  context){
    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        if(node->children[i] != NULL) {

            switch(node->children[i]->type){
                case IF_NODE:
                case WHILE_NODE:
                    ret = parseNode(node->children[i], context);
                    if(ret < 0)
                        return -1;
                    break;
                case INTEGER_CONSTANT_NODE:
                    parse("%d",node->children[i]->value.integer);
                    return 0;
                case FLOAT_CONSTANT_NODE:
                    parse("%f",node->children[i]->value.decimal);
                    return 0;

                case FUNCTION_CALL_NODE:
                    /* separator */
                case INTEGER_VARIABLE_CREATION_NODE:
                case STRING_VARIABLE_CREATION_NODE:
                case FLOAT_VARIABLE_CREATION_NODE:
                case BOOLEAN_VARIABLE_CREATION_NODE:
                    /* separator */
                case NUMERIC_VARIABLE_UPDATE_NODE:
                case STRING_VARIABLE_UPDATE_NODE:
                case BOOLEAN_VARIABLE_UPDATE_NODE:
                case IDENTIFIER_VARIABLE_UPDATE_NODE:
                    /* separator */
                case INTEGER_CONSTANT_CREATION_NODE:
                case STRING_CONSTANT_CREATION_NODE:
                case FLOAT_CONSTANT_CREATION_NODE:
                case BOOLEAN_CONSTANT_CREATION_NODE:
                    ret = parseNode(node->children[i], context);
                    if(ret < 0)
                        return -1;
                    break;
                default:
                    logDebug("Impossible children type: %s in CODE_BLOCK_NODE\n", NODE_NAMES[node->children[i]->type]);
                    return -1;
            }
        }
    }
    return ret;
}