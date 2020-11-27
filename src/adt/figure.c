#include "figure.h"
#include <stdlib.h>
#include "../utils/logger.h"
#include "../utils/parser.h"
#include "vector3.h"

typedef struct {
    FigureAttributeType type;
    FigureAttributeValue value;
} FigureAttribute;

struct figureCDT
{
    char * name;
    FigureType type;
    size_t attributes_count;
    FigureAttribute * attributes;
};

void drawBox(Figure figure);
void drawSphere(Figure figure);
void drawPyramid(Figure figure);
void drawComposite(Figure figure);
void drawCustom(Figure figure);


Figure newFigure(char * name, FigureType type){
    Figure figure = malloc(sizeof(struct figureCDT) );
    if(figure == NULL){
        logError(ERROR, "figure.c: Cannot allocate memory");
        return NULL;
    }

    figure -> name = name;      //TODO: Should strcpy?
    figure -> type = type;
    figure -> attributes_count = 0;
    figure -> attributes = NULL;

    return figure;
}

void setFigureAttribute(Figure figure, FigureAttributeType attr, FigureAttributeValue val){
    figure -> attributes_count++;
    figure -> attributes = realloc(figure -> attributes, figure -> attributes_count * sizeof(FigureAttribute));
    figure -> attributes[figure -> attributes_count-1].type = attr;  
    figure -> attributes[figure -> attributes_count-1].value = val;  
}

void parseFigure(Figure figure){
    switch (figure -> type)
    {
    case FIGURE_BOX:
        drawBox(figure);
        break;
    case FIGURE_SPHERE:
        drawSphere(figure);
        break;
    case FIGURE_PYRAMID:
        drawPyramid(figure);
        break;
    case FIGURE_COMPOSITE:
        drawComposite(figure);
        break;
    case FIGURE_CUSTOM:
        drawCustom(figure);
        break;
    default:
        break;
    }
}

Vector3 getFigurePosition(Figure figure){
    for(size_t i = 0; i < figure->attributes_count; i++){
        if(figure -> attributes[i].type == ATTR_POSITION){
            return figure -> attributes[i].value.vector;
        }
    }
    return NULL;
}

Vector3 getFigureScale(Figure figure){
    for(size_t i = 0; i < figure->attributes_count; i++){
        if(figure -> attributes[i].type == ATTR_SCALE){
            return figure -> attributes[i].value.vector;
        }
    }
    return NULL;
}

Vector3 getFigureRotation(Figure figure){
    for(size_t i = 0; i < figure->attributes_count; i++){
        if(figure -> attributes[i].type == ATTR_ROTATION){
            return figure -> attributes[i].value.vector;
        }
    }
    return NULL;
}

Vector3 getFigureColor(Figure figure){
    for(size_t i = 0; i < figure->attributes_count; i++){
        if(figure -> attributes[i].type == ATTR_COLOR){
            return figure -> attributes[i].value.vector;
        }
    }
    return NULL;
}

void freeFigure(Figure figure){
    logInfo("WARNING: freeFigure() not implemented.\n");
}

char *getFigureName(Figure figure) {
    return figure->name;
}

void drawBox(Figure figure){
    Vector3 pos = getFigurePosition(figure);
    Vector3 rot = getFigurePosition(figure);
    Vector3 scale = getFigurePosition(figure);
    Vector3 color = getFigureColor(figure);
    parse("\n-- Box: %s--\n", figure -> name);
    parse("pushMatrix();\n");
    parse("translate(%f,%f,%f);\n", pos->x, pos->y, pos->z);
    parse("fill(%f,%f,%f);\n", color->x, color->y, color->z);
    parse("rotate(%f,%f,%f);\n", rot->x, rot->y, rot->z);
    parse("box(%f,%f,%f);\n", scale->x, scale->y, scale->z);
    parse("popMatrix();\n");
}

void drawSphere(Figure figure){

}

void drawPyramid(Figure figure){

}

void drawComposite(Figure figure){

}

void drawCustom(Figure figure){

}

const char* figureTypeToString(FigureType type){
    switch (type) {
        case FIGURE_BOX:
            return "box";
        case FIGURE_SPHERE:
            return "sphere";
        case FIGURE_PYRAMID:
            return "pyramid";
        case FIGURE_COMPOSITE:
            return "composite";
        case FIGURE_CUSTOM:
            return "custom";
        case FIGURE_INVALID:
            return "invalid";
        default:
            return "null";
    }
}

void printAttribute(FigureAttribute attribute){
    switch (attribute.type) {
        case ATTR_SCALE:
            logInfo("Scale: ");
            printVector3(attribute.value.vector);
            logInfo("\n");
            break;
        case ATTR_POSITION:
            logInfo("Position: ");
            printVector3(attribute.value.vector);
            logInfo("\n");
            break;
        case ATTR_ROTATION:
            logInfo("Rotation: ");
            printVector3(attribute.value.vector);
            logInfo("\n");
            break;
        case ATTR_COLOR:
            logInfo("Color: ");
            printVector3Int(attribute.value.vectorInt);
            logInfo("\n");
            break;
        case ATTR_CHILD:
            logInfo("Child Figure: %s\n", attribute.value.figure->name);
            break;
        case ATTR_PATH:
            logInfo("Path: %s\n", attribute.value.path);
            break;
        default:
        case ATTR_INVALID:
            logInfo("Attr: Invalid Attr\n");
            return;
    }
}

void printFigure(Figure figure){
    logInfo("--| Figure: %s (Type: %s) |--\n", figure->name, figureTypeToString(figure->type));
    for (size_t i = 0; i < figure->attributes_count; ++i) {
        printAttribute(figure->attributes[i]);
    }
    logInfo("|-- Figure: %s (Type: %s) --|\n", figure->name, figureTypeToString(figure->type));
}