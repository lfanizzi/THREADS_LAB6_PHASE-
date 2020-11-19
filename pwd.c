#include "sh.h"

void pwd(){
    char *workingDirectory;

	workingDirectory = getcwd(NULL, 0);
    printf("%s\n", workingDirectory);
    free(workingDirectory);
}