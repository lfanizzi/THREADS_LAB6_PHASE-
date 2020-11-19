/*
Description: This function gets the path given the path variable 
*/

#include <string.h>
#include "get_path.h"

struct pathelement *get_path(){
  char *path, *p, *token;

  struct pathelement *current, *pathlist = NULL;

  p = getenv("PATH");	

  path = malloc((strlen(p)+1)*sizeof(char));
  strncpy(path, p, strlen(p));
  path[strlen(p)] = '\0';

  token = strtok(path, ":");

  do{
    if ( !pathlist ){
      current = calloc(1, sizeof(struct pathelement));
      pathlist = current;
    }else{
      current->next = calloc(1, sizeof(struct pathelement));
      current = current->next;
    }

    current->element = calloc(1, strlen(p)+1);	
    strcpy(current->element, token);
    current->next = NULL;

    token = strtok(NULL, ":");

  } while ( token != NULL );

  return pathlist;
} /* end get_path() */