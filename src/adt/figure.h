#ifndef _FIGURE_H_
#define _FIGURE_H_

#include "vector3.h"

typedef struct figureCDT * Figure;

typedef enum {
    FIGURE_BOX = 0,
    FIGURE_SPHERE,
    FIGURE_PYRAMID,
    FIGURE_COMPOSITE,
    FIGURE_CUSTOM,
    FIGURE_INVALID
} FigureType;

typedef enum {
    ATTR_SCALE = 0,
    ATTR_POSITION,
    ATTR_ROTATION,
    ATTR_COLOR,
    ATTR_CHILD,
    ATTR_PATH,
    ATTR_INVALID
} FigureAttributeType;

typedef union 
{
    Vector3 vector;
    Vector3Int vectorInt;
    Figure figure;
    char * path;
} FigureAttributeValue;


Figure newFigure(char * name, FigureType type);

void setFigureAttribute(Figure figure, FigureAttributeType attr, FigureAttributeValue val);

char * getFigureName(Figure figure);

void drawFigure(Figure figure);

void translateFigure(Figure figure, Vector3 vector3);

void rotateFigure(Figure figure, Vector3 vector3);

void scaleFigure(Figure figure, Vector3 vector3);

void addColorFigure(Figure figure, Vector3Int vector3Int);

void parseFigureAttributes(Figure figure);

void printFigure(Figure figure);

void freeFigure(Figure figure);

#endif /* _FIGURE_H_ */
