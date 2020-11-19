/*
Description: This function is responsible for killing functions based on arguments given
*/

#include "sh.h"
#include <stdlib.h>
#include <signal.h>

void killProc(char **arguments){
    printf("Executing built-in kill\n");

    int killSignal; // defines kill signal

    if(arguments[1] == NULL){ //checks if there are no arguments
        printf("Not enough arguments");
    }else if(arguments[1][0] == '-'){ // checks for special character "-"
       killSignal = atoi(&arguments[1][1]); // sets kill signal
       printf("processKilled: %d, with signal: %d\n", atoi(arguments[2]), (int)killProc); //kill confirmed
       kill(atoi(arguments[2]), killSignal); //kills signal
    }else{
        printf("processKilled: %d", atoi(arguments[1])); //kill confirmed
        kill(atoi(arguments[1]), SIGTERM); //kills signal
    }
}