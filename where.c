#include "get_path.h"
#include <string.h>
/*
    This function returns the paths to where the command lies.
    Input:
        char * command - This is the string command that is being found.
        struct pathelement *path - This is the path where the where command will search for.
*/
char **where(char *command, struct pathelement *path){
    char aPath[16][64];
    char **pathsFound;
    char *Found[16];
    int commandsFound = 0;

    while (path) {
        sprintf(aPath[commandsFound], "%s/%s", path->element, command);

        if (access(aPath[commandsFound], X_OK) == 0) {
            Found[commandsFound] = calloc(strlen(aPath[commandsFound]) + 1, sizeof(char));
            commandsFound++;
        }else{
            for(int index=0; index<64; index++){
                aPath[commandsFound][index] = '0';
            }
        }

        path = path->next;
    }

    for(int index = 0; index<commandsFound; index++){
        printf("%s\n", aPath[index]);
    }

    if (commandsFound) {
        pathsFound = calloc(commandsFound + 1, sizeof(char *));

        for(int index = 0; index<commandsFound; index++){
            pathsFound[index] = Found[index];
        }

        return pathsFound;
    }else{
        return (char **) NULL;
    }
}