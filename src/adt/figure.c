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

char * getFigurePath(Figure figure){
    for(size_t i = 0; i < figure->attributes_count; i++){
        if(figure->attributes[i].type == ATTR_PATH){
            return  figure->attributes[i].value.path;
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

void parseAttribute(Figure figure, FigureAttribute attr){
    switch (attr.type) {
        case ATTR_SCALE:
            parse("%s.fscale = new Vector3(%f,%f,%f);\n", figure->name, attr.value.vector->x, attr.value.vector->y, attr.value.vector->z);
            break;
        case ATTR_POSITION:
            parse("%s.fposition = new Vector3(%f,%f,%f);\n", figure->name, attr.value.vector->x, attr.value.vector->y, attr.value.vector->z);
            break;
        case ATTR_ROTATION:
            parse("%s.frotation = new Vector3(%f,%f,%f);\n", figure->name, attr.value.vector->x, attr.value.vector->y, attr.value.vector->z);
            break;
        case ATTR_COLOR:
            parse("%s.fcolor = new Vector3Int(%d,%d,%d);\n", figure->name, attr.value.vectorInt->x, attr.value.vectorInt->y, attr.value.vectorInt->z);
            break;
        case ATTR_CHILD:
            parse("%s.fchildren.add(%s);\n",figure->name, attr.value.figure->name);
            break;
        case ATTR_PATH:
            parse("%s.fpath = \"%s\";\n",figure->name, attr.value.path);
            break;
        case ATTR_INVALID:
            break;
    }
}

void parseFigureAttributes(Figure figure) {
    for(size_t i = 0; i < figure->attributes_count; i++){
        parseAttribute(figure, figure->attributes[i]);
    }
    parse("\n");
}

void drawBox(Figure figure){
    parse("pushMatrix();\n");
    parse("translate(%s.fposition.x,%s.fposition.y,%s.fposition.z);\n", figure -> name, figure -> name, figure -> name);
    parse("fill(%s.fcolor.x,%s.fcolor.y,%s.fcolor.z);\n", figure -> name, figure -> name, figure -> name);
    parse("rotateX(radians(%s.frotation.x)); rotateY(radians(%s.frotation.y)); rotateZ(radians(%s.frotation.z));\n", figure -> name, figure -> name, figure -> name);
    parse("scale(%s.fscale.x,%s.fscale.x,%s.fscale.z);\n", figure -> name, figure -> name, figure -> name);
    parse("box(1);\n");
    parse("popMatrix();\n\n");
}

void drawSphere(Figure figure){
    parse("pushMatrix();\n");
    parse("translate(%s.fposition.x,%s.fposition.y,%s.fposition.z);\n", figure -> name, figure -> name, figure -> name);
    parse("fill(%s.fcolor.x,%s.fcolor.y,%s.fcolor.z);\n", figure -> name, figure -> name, figure -> name);
    parse("rotateX(radians(%s.frotation.x)); rotateY(radians(%s.frotation.y)); rotateZ(radians(%s.frotation.z));\n", figure -> name, figure -> name, figure -> name);
    parse("scale(%s.fscale.x,%s.fscale.y,%s.fscale.z);\n", figure -> name, figure -> name, figure -> name);
    parse("sphere(1);\n");
    parse("popMatrix();\n\n");
}

void drawPyramid(Figure figure){
    parse("pushMatrix();\n");
    parse("translate(%s.fposition.x,%s.fposition.y,%s.fposition.z);\n", figure -> name, figure -> name, figure -> name);
    parse("fill(%s.fcolor.x,%s.fcolor.y,%s.fcolor.z);\n", figure -> name, figure -> name, figure -> name);
    parse("rotateX(radians(%s.frotation.x)); rotateY(radians(%s.frotation.y)); rotateZ(radians(%s.frotation.z));\n", figure -> name, figure -> name, figure -> name);
    parse("scale(%s.fscale.x,%s.fscale.y,%s.fscale.z);\n", figure -> name, figure -> name, figure -> name);
    parse("beginShape(TRIANGLES);\n"
          "\n"
          "vertex(-1, -1, -1);\n"
          "vertex( 1, -1, -1);\n"
          "vertex( 0, 0, 1);\n"
          "\n"
          "vertex( 1, -1, -1);\n"
          "vertex( 1, 1, -1);\n"
          "vertex( 0, 0, 1);\n"
          "\n"
          "vertex( 1, 1, -1);\n"
          "vertex(-1, 1, -1);\n"
          "vertex( 0, 0, 1);\n"
          "\n"
          "vertex(-1, 1, -1);\n"
          "vertex(-1, -1, -1);\n"
          "vertex( 0, 0, 1);\n"
          "\n"
          "  endShape();\n");
    parse("popMatrix();\n\n");
}

void drawComposite(Figure figure){
    parse("pushMatrix();\n");
    parse("translate(%s.fposition.x,%s.fposition.y,%s.fposition.z);\n", figure -> name, figure -> name, figure -> name);
    parse("rotateX(radians(%s.frotation.x)); rotateY(radians(%s.frotation.y)); rotateZ(radians(%s.frotation.z));\n", figure -> name, figure -> name, figure -> name);
    parse("scale(%s.fscale.x,%s.fscale.y,%s.fscale.z);\n", figure -> name, figure -> name, figure -> name);
    for(size_t i = 0; i < figure->attributes_count; i++){
        if(figure->attributes[i].type == ATTR_CHILD){
            drawFigure(figure->attributes[i].value.figure);
        }
    }
    parse("popMatrix();\n\n");
}

void drawCustom(Figure figure){
    char * path = getFigurePath(figure);
    if(path != NULL) {
        /*parse("pushMatrix();\n");
        parse("translate(%s.fposition.x,%s.fposition.y,%s.fposition.z);\n", figure->name, figure->name, figure->name);
        parse("rotateX(radians(%s.frotation.x)); rotateY(radians(%s.frotation.y)); rotateZ(radians(%s.frotation.z));\n",
              figure->name, figure->name, figure->name);
        parse("scale(%s.fscale.x,%s.fscale.y,%s.fscale.z);\n", figure->name, figure->name, figure->name);
        parse("loadShape(\"%s\");\n", path);
        parse("popMatrix();\n\n");*/
        parse("// Custom Shape not implemented\n\n");
    } else{
        logWarning("Custom figure %s does not contain a path. Skipping draw.\n");
    }
}

void drawFigure(Figure figure){
    switch (figure->type)
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
            logInfo("ERROR: parseDrawFigure(): Invalid figure type %s.", figureTypeToString(figure->type));
    }
}

void printFigure(Figure figure){
    logInfo("--| Figure: %s (Type: %s) |--\n", figure->name, figureTypeToString(figure->type));
    for (size_t i = 0; i < figure->attributes_count; ++i) {
        printAttribute(figure->attributes[i]);
    }
    logInfo("|-- Figure: %s (Type: %s) --|\n", figure->name, figureTypeToString(figure->type));
}
