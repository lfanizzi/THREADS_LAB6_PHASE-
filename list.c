#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include "sh.h"

/*
    This function just lists the files in the current directory 
    or all the files in the directory listed.
    Input:
        char **arguments - argueemtns typd into the shell
    returns:
        void
*/
void list(char **arguments){

    char directoryName[MAXLINE];
    DIR *directory; //defines DIR
    struct dirent *d; // defines dirent pointer

    char *workingDirectory;

    for(int index = 1; arguments[index] != NULL; index++){
        //prints all arguments, except if it hits a pipe or a file redirect.
        //must do this to print all arguments if user inputs *.
        if(arguments[index][0] == '|' || arguments[index][0] == '>'){
            break;
        }
        printf("%s\n", arguments[index]); 
    }

    //looks to see if the file is a directory, if it is it prints it.
    //IMPORTANT: assumes this is run within your current working directory.
    for(int index = 1; arguments[index] != NULL; index++){
        workingDirectory = getcwd(NULL, 0);

        strcpy(directoryName, workingDirectory); //copies directory name onto string

        if(arguments[index][0] != '/'){ //checks if there is a / character
            strcat(directoryName, "/");  //concatinates / and the name
        }

        strcat(directoryName, arguments[index]);

        directory = opendir(directoryName); // sets directory

        if(directory != NULL){

            printf("\n");
            printf("%s:\n", arguments[index]);
            //prints all of the files in the directory.
            d = readdir(directory); 
            while(d != NULL){
                if(d->d_name[0] != '.' ){
                    printf("%s\n", d->d_name);
                }

                d = readdir(directory);
            }
        }
    }

    //just prints all of the files in the current directory if the second argumnt is null, pipe or file redirect.
    if(arguments[1] == NULL || arguments[1][0] == '|' || arguments[1][0] == '>'){
        directory = opendir(getcwd(NULL, 0));

        d = readdir(directory);
        while(d != NULL){

            if(d->d_name[0] != '.' ){
                printf("%s\n", d->d_name);
            }

            d = readdir(directory);
        }

    }

    if(directory != NULL){
        closedir(directory);
    }


}

