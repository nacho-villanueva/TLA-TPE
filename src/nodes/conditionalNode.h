#ifndef _CONDITIONAL_NODE_H_
#define _CONDITIONAL_NODE_H_

#include "node.h"

int parseOrAndConditionalNode(Node * node, U3D_Context *  context);

int parseNumericConditionalNode(Node * node, U3D_Context *  context);

int parseStringConditionalNode(Node * node, U3D_Context *  context);

int parseBooleanConditionalNode(Node * node, U3D_Context *  context);

int parseDoubleIdentifierConditionalNode(Node * node, U3D_Context *  context);

#endif