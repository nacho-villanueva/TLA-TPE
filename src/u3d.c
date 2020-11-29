#include "u3d.h"
#include "core/U3D_Functions.h"
#include "configs.h"
#include "utils/logger.h"
#include "utils/parser.h"
#include "utils/args.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define PROCESSING_JAVA "processing-java"

struct u3d_settings {
    const char* u3dre_path;
    FILE * inputFile;
    char outputFile[MAX_PATH_SIZE];
    char outputDir[MAX_PATH_SIZE];
    bool compileToSource;
    const char * compilationPlatform;
};

U3D * initU3D(int argc, char * argv[]){
    logDebug(" ---- Init U3D ----\n");

    U3D * settings = calloc(1, sizeof(struct u3d_settings));


    if(argc < 2) {
        logError(ERROR, "Invalid arguments amount.\n");
        closeU3D(settings);
        return NULL;
    }

    if(settings == NULL){
        logError(ERROR, "u3d_settings: Cannot allocate memory.\n");
        closeU3D(settings);
        return NULL;
    }


    /* ----- SET U3D REALTIME ENVIROMENT ----- */
    const char* u3dre_path = getenv(U3DRE_ENV_VAR);
    if(u3dre_path == NULL){
        u3dre_path = U3DRE_DEFAULT_PATH;
        logWarning("%s enviroment variable not found. Using default U3D Runtime Enviroment path.\n", U3DRE_ENV_VAR);
    }

    char processingPath[MAX_PATH_SIZE];
    int len = snprintf(processingPath, MAX_PATH_SIZE-1, "%s/%s", u3dre_path, PROCESSING_JAVA);
    if(len > MAX_PATH_SIZE-1)
        logDebug("WARNING: Insufficient buffer size for U3DRE path.\n", U3DRE_ENV_VAR);

    if(access(processingPath, F_OK ) == -1){
        logError(FATAL_ERROR, "U3D Runtime Enviroment not found.\n");
        closeU3D(settings);
        return NULL;
    }

    settings -> u3dre_path = u3dre_path;

    /* ----- OPEN INPUT FILE ----- */
    char * inputFile = getInputFile(argc, argv);
    settings -> inputFile = fopen(inputFile, "r");
	if (!settings -> inputFile) { 
		logError(FATAL_ERROR, "No such input file: %s.\n", inputFile);
        closeU3D(settings);
		return NULL; 
	} 

    /* ----- CREATE OUTPUT DIRECTORY ----- */
    char * outputDir = getOutputFile(argc, argv);
    if(outputDir != NULL){
        len = strlen(outputDir);
        if(len >= MAX_PATH_SIZE - 5){
            logError(ERROR, "Output file excedes buffer limit\n");
            closeU3D(settings);
            return NULL;
        }
        strcpy(settings -> outputDir, outputDir);
    }
    else{
        strcpy(settings -> outputDir, DEFAULT_OUTPUT_DIR);
    }

    struct stat st = {0};
    logDebug("Creating output folder: %s\n", settings -> outputDir);
    if (stat(settings -> outputDir, &st) == -1) {
        if(mkdir(settings -> outputDir, 0700) < 0){
            logError(ERROR, "Could not create output directory: %s\n", settings -> outputDir);
            closeU3D(settings);
            return NULL;
        }
    } else {
        char cmd[2048];
        char cwd[256];
        getcwd(cwd, 256);
        snprintf(cmd, 2048, "rm -f -r %s/%s/*", cwd, settings -> outputDir);
        logDebug("Output folder already exists. Cleaning up contents.\n", cmd);
        logDebug("SYSTEM COMMAND: %s\n", cmd);
        int rmret = system(cmd);
        if(rmret != 0){
            logWarning("Verify your previous build files are not opened by another program.\n");
        }
    }

    /* ----- OPEN OUTPUT FILE ----- */
    char * outputFile = getOutputFile(argc, argv);
    if(outputFile != NULL){
        size_t n = snprintf(settings -> outputFile, MAX_PATH_SIZE-1, "%s/%s.pde", settings->outputDir, outputFile);
        if(n > MAX_PATH_SIZE){
            logError(ERROR, "Output file excedes buffer limit\n");
            closeU3D(settings);
            return NULL;
        }
    } else {
        strcpy(settings -> outputFile, DEFAULT_OUTPUT_FILE);
    }

    if(initParser(settings -> outputFile) < 0){
        closeU3D(settings);
        return NULL;
    }

    settings->compileToSource = getCompilationType(argc, argv);
    settings->compilationPlatform = getCompilationPlatform(argc, argv);

    logDebug(" ---- YACC ----\n");

    return settings;
}

int compileU3D(U3D * settings, Node * root){

    logDebug(" ---- Parsing Nodes ----\n");

    U3D_Context context;
    memset(&context, 0, sizeof(U3D_Context));

    initU3DFunctions(&context);

    if(parseNode(root, &context) < 0){
        return -1;
    }

    if(!settings->compileToSource) {
        logDebug(" ---- Compiling Processing Code ----\n");

        char cmd[CMD_BUFFER_SIZE];
        char cwd[MAX_PATH_SIZE];
        getcwd(cwd, MAX_PATH_SIZE);

        // Compile Processing File
        snprintf(cmd, CMD_BUFFER_SIZE, "%s/%s --sketch=%s/%s --output=%s/%s/%s --platform=%s --force --export", settings->u3dre_path,
                 PROCESSING_JAVA, cwd, settings->outputDir, cwd, settings->outputDir, "bin", settings->compilationPlatform);
        logDebug("Running Processing compiler:\n");
        logDebug("SYSTEM COMMAND: %s\n", cmd);
        system(cmd);

        // Clean Up
        snprintf(cmd, CMD_BUFFER_SIZE, "mv %s/%s/%s/* %s/%s && rm -f %s/%s/%s", cwd, settings->outputDir, "bin", cwd,
                 settings->outputDir, cwd, settings->outputDir, settings->outputFile);
        logDebug("Cleaning Up:\n");
        logDebug("SYSTEM COMMAND: %s\n", cmd);
        system(cmd);
        // Clean Up
        snprintf(cmd, CMD_BUFFER_SIZE, "rm -f %s/%s && rmdir %s/%s/%s", cwd, settings->outputFile, cwd, settings->outputDir, "bin");
        logDebug("SYSTEM COMMAND: %s\n", cmd);
        system(cmd);
    }

    return 0;   // TODO: manage errors
}


void closeU3D(U3D * settings) {
    if(settings != NULL){
        if(settings -> inputFile)
            fclose(settings->inputFile);
        free(settings);
    }
}

FILE * getU3DInputFile(U3D * settings){
    return settings -> inputFile;
}
