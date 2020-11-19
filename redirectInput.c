#include <unistd.h>  //included files and libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <glob.h>
#include <signal.h>
#include "sh.h"
#include <sys/stat.h>
#include <fcntl.h>

void redirectInput( char* command, char* fileName){
	int fid = open(fileName,O_WRONLY|O_CREAT|O_TRUNC);
	close(1); 
	dup2(fid,0); 
	close(fid);
    fid = open("/dev/tty", O_WRONLY);

}