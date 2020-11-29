#include "rootNodes.h"
#include <stdio.h>
#include "../utils/parser.h"
#include "figureNode.h"

int parseBlock(Node * node, U3D_Context *  context, NodeType type){
    for(int i = 0; i < node -> childrenCount; i++){
        if(node -> children[i] -> type == type){
            return parseNode(node -> children[i], context);
        }
    }
    return 0;
}

void addVector3Class(){
    parse("class Vector3 {\n"
          "\tfloat x, y, z;\n"
          "\tpublic Vector3(){x=0;y=0;z=0;}\n"
          "\tpublic Vector3(float x, float y, float z){this.x=x;this.y=y;this.z=z;}\n"
          "}\n\n"
    );
}

void addVector3IntClass(){
    parse("class Vector3Int {\n"
          "\tint x, y, z;\n"
          "\tpublic Vector3Int(){x=0;y=0;z=0;}\n"
          "\tpublic Vector3Int(int x, int y, int z){this.x=x;this.y=y;this.z=z;}\n"
          "}\n\n"
    );
}

void addFigureClass(){
    parse(
            "class Figure {\n"
            "\tVector3 fposition = new Vector3();\n"
            "\tVector3 fscale = new Vector3();\n"
            "\tVector3 frotation = new Vector3();\n"
            "\tVector3Int fcolor = new Vector3Int();\n"
            "\tString fpath;\n"
            "\tArrayList<Figure> fchildren = new ArrayList<Figure>();\n"
            "}\n\n");
}

int parseRootNode(Node * node, U3D_Context *  context){
    int ret = 0;

    addVector3IntClass();
    addVector3Class();
    addFigureClass();

    // Parse in order and only parses each block once, despite multiple declarations
    ret += parseBlock(node, context, DEFINITIONS_NODE);
    ret += parseBlock(node, context, SETTINGS_NODE);
    ret += parseBlock(node, context, DRAW_NODE);

    return ret;
}

int parseDefinitionsNode(Node * node, U3D_Context *  context){
    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }
    return ret;
}

int parseSettingsNode(Node * node, U3D_Context * context){
    parse("\nvoid setup() { \n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }

    parse("size(800,600, P3D);\nnoStroke();\n\n");

    parseFiguresInit(context);

    parse("\n}\n");
    return 0;
}

int parseDrawNode(Node * node, U3D_Context * context){
    parse("\nvoid draw() { \n");

    parse("pointLight(51, 102, -126, 400, 400, -250);\n\n");

    /*for(size_t i = 0; i < context->figuresCount; i++){
        parseDrawFigure(context->figuresTable[i]);
    } // DRAW ALL FIGURES IN TABLE */

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }

    parse("\n}\n");
    return 0;
}