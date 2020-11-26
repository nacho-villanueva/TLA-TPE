#include <string.h>
#include <stdlib.h>

char * getOutputFile(int argc, char * argv[]) {
    for(int i = 0; i < argc-1; i++){
        if(strcmp(argv[i], "-o") == 0 || strcmp(argv[1], "--output") == 0)
            return argv[i+1]; 
    }
    return NULL;
}

char * getInputFile(int argc, char * argv[]){
    return argv[argc-1];
}
