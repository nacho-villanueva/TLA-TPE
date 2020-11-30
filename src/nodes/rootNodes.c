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
            "\tVector3 fscale = new Vector3(50,50,50);\n"
            "\tVector3 frotation = new Vector3();\n"
            "\tVector3Int fcolor = new Vector3Int();\n"
            "\tString fpath;\n"
            "\tArrayList<Figure> fchildren = new ArrayList<Figure>();\n"
            "}\n\n");
}

int parseRootNode(Node * node, U3D_Context *  context){

    addVector3IntClass();
    addVector3Class();
    addFigureClass();

    parse("Vector3Int _backgroundColor = new Vector3Int(201,201,201);\n");
    parse("boolean _ColorModeHSB = false;\n");
    parse("boolean _LightsOn = true;\n\n");

    // Parse in order and only parses each block once, despite multiple declarations
    if(parseBlock(node, context, DEFINITIONS_NODE) < 0)
        return -1;
    if(parseBlock(node, context, SETTINGS_NODE) < 0)
        return -1;
    if(parseBlock(node, context, DRAW_NODE) < 0)
        return -1;

    return 0;
}

int parseDefinitionsNode(Node * node, U3D_Context *  context){
    int ret = 0;

    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i], context);
    }
    return ret;
}

int parseSettingsNode(Node * node, U3D_Context * context){
    parse("\nvoid settings() { \n");
    parse("size(1280,720,P3D);\n");
    parse("smooth(8);\n");

    Node * child = getChildNode(node, SETTINGS_LIST_NODE);
    if(child != NULL){
        parseNode(child, context);
    }

    parse("\n}\n");


    parse("\nvoid setup() { \n");
    parse("if(_ColorModeHSB) colorMode(HSB, 360, 100, 100);\n\n");

    parseFiguresInit(context);

    parse("\n}\n");

    return 0;
}

int parseSettingsListNode(Node * node, U3D_Context * context){
    for(int i = 0; i < node->childrenCount; i++){
        if(parseNode(node->children[i], context) < 0)
            return -1;
    }
    return 0;
}

int parseDrawNode(Node * node, U3D_Context * context){
    parse("\nvoid draw() { \n");
    parse("background(_backgroundColor.x, _backgroundColor.y, _backgroundColor.z);\n");
    parse("if(_LightsOn) lights();\n");
    parse("translate(width/2, height/2);\n\n");

    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret = parseNode(node->children[i], context);
        if(ret < 0)
            return ret;
    }

    parse("\n}\n");
    return ret;
}