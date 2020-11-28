#ifndef _U3DDEF_H_
#define _U3DDEF_H_

#include <stdlib.h>
#include <stdint.h>
#include "adt/figure.h"
#include "adt/variable.h"

typedef struct {
    size_t figuresCount;
    Figure * figuresTable;
    Variable first;
} U3D_Context;

bool isVariableDefined(char * name, U3D_Context * context);

int addFigureToTable(Figure figure, U3D_Context * context);

Figure getFigureFromTable(char* name, U3D_Context * context);

int newVariable(char * identifier, enum VariableType type, union VariableValue value, U3D_Context * context);

#endif /* _U3DDEF_H_ */
