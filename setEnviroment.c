#include <stdlib.h> 
#include "sh.h"
/*
Description: This function is responsible for setting the environment when called. It can take in two double
pointers to characters.
*/
void setEnvironment(char ** environment, char **arguments){
    int index = 0;
    if(arguments[1] == NULL){ //checks there are no arguments
        while(environment[index]){
            printf("%s\n", environment[index]); //prints the environment by indexing through it
            index++;
        }
    }else if(arguments[1] != NULL ){  
    if(arguments[2]==NULL){ //cheecks there is only one argument
        setenv(arguments[1], "" , 1);//sets environment variable to empty
    }}else if(arguments[3]!= NULL){ //checks if there are too many varriables
        printf("Too many arguments\n"); //error message
    }else if(arguments[1] != NULL && arguments[2] != NULL){ //checks that there are only two arguments
        setenv(arguments[1],arguments[2],1); // the second one is the value of the first, sets environment accordingly
    }
}

