#ifndef _CONDITIONAL_NODE_H_
#define _CONDITIONAL_NODE_H_

#include "node.h"

int parseOrAndNode(Node * node);

int parseNumericConditionalNode(Node * node);

int parseStringConditionalNode(Node * node);

int parseBooleanConditionalNode(Node * node);

#endif