#ifndef _FIGURENODE_H_
#define _FIGURENODE_H_

#include "node.h"

int parseFigureNode(Node * node, U3D_Context * context);

void parseFiguresInit(U3D_Context * context);

int parseDrawFigure(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseTranslateFigure(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseRotateFigure(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseScaleFigure(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseAddColorFigure(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseSetFigurePosition(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseSetFigureRotation(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseSetFigureScale(size_t paramCount, ParameterValue * values, ParameterType * types);
int parseSetFigureColor(size_t paramCount, ParameterValue * values, ParameterType * types);




#endif /* _FIGURENODE_H_ */
