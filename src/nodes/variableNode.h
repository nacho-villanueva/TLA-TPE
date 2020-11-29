#ifndef _VARIABLE_NODE_H_
#define _VARIABLE_NODE_H_

#include "node.h"

int parseVariableCreationNode(Node * node, U3D_Context *  context, bool isConstant);

int parseVariableUpdateNode(Node * node, U3D_Context *  context);

#endif