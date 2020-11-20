#include "node.h"
#include <stdio.h>

#define IMPORTS "import processing.core.*;import processing.data.*;import processing.event.*;import processing.opengimport java.util.HashMap; import java.util.ArrayList; import java.io.File; import java.io.BufferedReader; import java.io.PrintWriter; import java.io.InputStream; import java.io.OutputStream; import java.io.IOException;" 

int parseRootNode(Node * node){
    printf("%s\n", IMPORTS);
    printf("\npublic class test extends PApplet {\n");
    int ret = 0;
    for(int i = 0; i < node->childrenCount; i++){
        ret += parseNode(node->children[i]);
    }
    printf("\n}\n");
    return ret;
}