#ifndef _U3D_H_
#define _U3D_H_

#include <stdio.h>
#include "nodes/node.h"

#define U3DRE_ENV_VAR "U3DRE_PATH"
#define U3DRE_DEFAULT_PATH "u3dre"

typedef struct u3d_settings U3D;

U3D * initU3D();

int compileU3D(U3D * settings, Node * root);

void closeU3D(U3D * settings, Node * root);

FILE * getU3DInputFile(U3D * settings);

#endif //_U3D_H_