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




void redirectOutput2(char* command, char* fileName){
    int fid = open(fileName,O_WRONLY|O_CREAT|O_TRUNC);
    dup2(fid,1);
    dup2(fid,2);
	close(fid);
    fid = open("/dev/tty", O_WRONLY); // back to terminal

}