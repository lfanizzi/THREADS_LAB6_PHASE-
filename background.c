#include <unistd.h> //included files and libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <glob.h>
#include <signal.h>
#include "sh.h"
#include <sys/stat.h>
#include <fcntl.h>

pid_t background(char **arguments, int argumentIndex)
{
	int status = 0;

	printf("In background: \n");
	pid_t child = fork();
	//printf("Checking");
	if (child == 0)
	{
		struct pathelement *path = get_path();
		char *cmd = which(arguments[0], path);
		signal(SIGINT, SIG_IGN); //ignores ctrl.c for background process
		setenv("parent",getcwd(cmd, 1024),1);
		execve(cmd, arguments, NULL);
		execve(arguments[0], arguments, NULL);
		perror("Background Process Failed");
	
	}else
	{
		//waitpid(getpid(),NULL,WNOHANG);
		return 0;
		//kill(0,0);
		//exit(0);
	}

	return child; //child;
}
