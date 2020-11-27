#ifndef _U3DDEF_H_
#define _U3DDEF_H_

#include <stdlib.h>
#include <stdint.h>
#include "adt/figure.h"

typedef struct {
    size_t figuresCount;
    Figure * figuresTable;
} U3D_Context;


#endif /* _U3DDEF_H_ */
