#include "get_path.h"

/*
  This function only searches for the first place the command shows up in the given path.
  Input:
    char * command - This is the command the which command is searching for.
    struct pathelement *path - This is the given path where the which command will search for the command.
  Return:
    char * - The first path where the command is located.

*/
char *which(char *command, struct pathelement *path){
  char cmd[64], *pathFound;
  int  found;

  found = 0;
  while (path) {

    sprintf(cmd, "%s/%s", path->element, command);

    if (access(cmd, X_OK) == 0) {
      found = 1;
      break;
    }

    path = path->next;
  }

  if (found) {
    pathFound = malloc(strlen(cmd)+1);
    strcpy(pathFound, cmd);
    return pathFound;
  }else{
    return (char *) NULL;
  }

}