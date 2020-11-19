/*
Description: This file contains function definitions and definitioins of important values
*/

#include "get_path.h"
#include "linkedList.c"

int pid;
char *which(char *command, struct pathelement *pathlist);
char **where(char *command, struct pathelement *pathlist);
void list(char **arguments);
void printenv(char ** environment, int maxArguments, char **arguments);
void setEnvironment(char ** environment, char **arguments);
void cd(char *cd);
void pwd();
void killProc(char **arguments);
void pipeCommand(char **arguments, int pipeIndex, int *fd);
void redirectOutput( char* command, char* fileName);
void redirectOutput2(char* command, char* fileName);
int noclobber(int noclobber);
void redirectAppend( char* command, char* fileName);
void redirectAppend2(char* command, char* fileName);
void redirectInput( char* command, char* fileName);
pid_t background(char** arguments, int argumentIndex);
void *watchUser();

#define OFF 0
#define ON 1
#define PROMPTMAX 64
#define MAXARGS   16
#define MAXLINE   128