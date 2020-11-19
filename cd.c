#include "sh.h"
#include <stdlib.h>
/*
Description: This function changes the directory based omn the amount of arguments given
*/
void cd (char *dir){

    printf("Executing built-in [cd]\n");

    char *oldwd = getcwd(NULL,0);
    if(strcmp(dir,"~") == 0){
        if(chdir(getenv("HOME")) != 0){
            perror("cd failed");
            return;
        }
    }else if(strcmp(dir,"-") == 0){
        if(chdir(getenv("OLDPWD"))!= 0){
            perror("cd failed"); 
            return;
        }
    }else{
        if(chdir(dir) != 0){
            perror("cd failed");
            return;
        }
    }

    if(setenv("OLDPWD", oldwd, 1) < 0){
            perror("cd failed");
        }

    if(setenv("PWD", getcwd(NULL,0), 1) < 0){
        perror("cd failed");  
    }
}