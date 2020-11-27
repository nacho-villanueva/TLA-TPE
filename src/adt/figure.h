#ifndef _FIGURE_H_
#define _FIGURE_H_

#include "vector3.h"

typedef struct figureCDT * Figure;

typedef enum {
    FIGURE_BOX = 0,
    FIGURE_SPHERE,
    FIGURE_PYRAMID,
    FIGURE_COMPOSITE,
    FIGURE_CUSTOM
} FigureType;

typedef enum {
    ATTR_SCALE = 0,
    ATTR_POSITION,
    ATTR_ROTATION,
    ATTR_COLOR,
    ATTR_CHILD,
    ATTR_PATH
} FigureAttributeType;

typedef union 
{
    Vector3 scale;
    Vector3 position;
    Vector3 rotation;
    Vector3 color;
    Figure child;
    char * path;
} FigureAttributeValue;


Figure newFigure(char * name, FigureType type);

void setFigureAttribute(Figure figure, FigureAttributeType attr, FigureAttributeValue val);

void parseFigure(Figure figure);

void freeFigure(Figure figure);

#endif /* _FIGURE_H_ */
