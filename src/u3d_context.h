#ifndef _U3DDEF_H_
#define _U3DDEF_H_
#include "adt/figure.h"
#include "adt/function.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "adt/figure.h"
#include "adt/variable.h"


typedef struct {
    size_t figuresCount;
    Figure * figuresTable;
    Variable first;

    size_t functionCount;
    Function * functionTable;
} U3D_Context;

bool isVariableDefined(char * name, U3D_Context * context);

int addFigureToTable(Figure figure, U3D_Context * context);

Figure getFigureFromTable(char* name, U3D_Context * context);

int addFunctionToTable(Function function, U3D_Context * context);

Function getFunctionFromTable(char * function, U3D_Context * context);

int newVariable(char * identifier, enum VariableType type, union VariableValue value, U3D_Context * context, bool isConstant);

#endif /* _U3DDEF_H_ */
