#include "u3d.h"
#include "logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <strings.h>

#define PROCESSING_JAVA "processing-java.exe"

struct _u3d_settings {
    const char* u3dre_path;
};


U3D * initU3D(){
    U3D * settings = malloc(sizeof(struct _u3d_settings));

    if(settings == NULL){
        logError(ERROR, "u3d_settings: Cannot allocate memory.\n");
        closeU3D(settings);
        return NULL;
    }

    const char* u3dre_path = getenv(U3DRE_ENV_VAR);
    if(u3dre_path == NULL){
        u3dre_path = U3DRE_DEFAULT_PATH;
        logWarning("%s enviroment variable not found. Using default U3D Runtime Enviroment path.\n", U3DRE_ENV_VAR);
    }

    const size_t bufSize = 256;
    char processingPath[bufSize];
    int len = snprintf(processingPath, bufSize-1, "%s/%s", u3dre_path, PROCESSING_JAVA);
    if(len > bufSize-1)
        logDebug("WARNING: Insufficient buffer size for U3DRE path.\n", U3DRE_ENV_VAR);

    if(access(processingPath, F_OK ) == -1){
        logError(FATAL_ERROR, "U3D Runtime Enviroment not found.\n");
        closeU3D(settings);
        return NULL;
    }
    return settings;
}

void closeU3D(U3D * settings) {
    if(settings != NULL)
        free(settings);
}
