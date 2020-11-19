#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct pathelement{
  char *element;
  struct pathelement *next;
};

struct pathelement *get_path();