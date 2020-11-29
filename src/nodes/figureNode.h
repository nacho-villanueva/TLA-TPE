#ifndef _FIGURENODE_H_
#define _FIGURENODE_H_

#include "node.h"

int parseFigureNode(Node * node, U3D_Context * context);

void parseFiguresInit(U3D_Context * context);

int parseDrawFigure(ParameterValue * values);


#endif /* _FIGURENODE_H_ */
