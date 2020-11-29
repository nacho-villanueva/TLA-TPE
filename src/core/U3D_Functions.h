#ifndef U3D_FUNCTIONS_H
#define U3D_FUNCTIONS_H

#include "../adt/function.h"
#include "../adt/figure.h"

#include "../u3d_context.h"
#include "../nodes/node.h"


void initU3DFunctions(U3D_Context * context);

int callU3DFunction(Node * node, U3D_Context * context);

#endif //U3D_FUNCTIONS_H
