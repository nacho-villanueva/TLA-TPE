#ifndef _ROOTNODES_H_
#define _ROOTNODES_H_

#include "node.h"

int parseRootNode(Node * node, U3D_Context *  context);

int parseDefinitionsNode(Node * node, U3D_Context * context);

int parseSettingsNode(Node * node, U3D_Context * context);

int parseSettingsListNode(Node * node, U3D_Context * context);

int parseDrawNode(Node * node, U3D_Context * context);


#endif /* _ROOTNODES_H_ */
