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




void redirectOutput( char* command, char* fileName){
	//printf("IM here");
	int fid = open(fileName,O_WRONLY|O_CREAT|O_TRUNC);
    //printf("CLOSINGTIME");
//	fid = open(“foo.txt”, O_WRONLY|O_CREAT|O_TRUNC); 
	close(1); 
	dup(fid); 
	close(fid);
    //printf("CLOSINGTIME");
    fid = open("/dev/tty", O_WRONLY);

}
