#ifndef _U3DDEF_H_
#define _U3DDEF_H_
#include "adt/figure.h"
#include "adt/function.h"

#include <stdlib.h>
#include <stdint.h>


typedef struct {
    size_t figuresCount;
    Figure * figuresTable;

    size_t functionCount;
    Function * functionTable;
} U3D_Context;

bool isVariableDefined(char * name, U3D_Context * context);

int addFigureToTable(Figure figure, U3D_Context * context);

Figure getFigureFromTable(char* name, U3D_Context * context);

int addFunctionToTable(Function function, U3D_Context * context);

Function getFunctionFromTable(char * function, U3D_Context * context);

#endif /* _U3DDEF_H_ */
