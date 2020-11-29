#include "functionCallNode.h"
#include "../utils/logger.h"
#include "../core/U3D_Functions.h"

int parseFunctionCallNode(Node * node, U3D_Context * context) {
    return callU3DFunction(node, context);
}
