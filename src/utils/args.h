#ifndef _ARGS_H_
#define _ARGS_H_

#include <stdbool.h>

char * getOutputFile(int argc, char * argv[]);

char * getInputFile(int argc, char * argv[]);

bool getCompilationType(int argc, char * argv[]);


#endif /* _ARGS_H_ */
