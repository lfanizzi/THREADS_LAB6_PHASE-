#include "sh.h"
#include <unistd.h>

void pipeCommand(char **arguments, int pipeIndex, int *fd)
{
    pipe(fd);
    pid = fork();

    if (pid < 0){
        perror("fork error");
    }else if (pid == 0){
        //child
        close(0);
        dup(fd[0]);
        close(fd[1]);

        execvp(arguments[pipeIndex + 1], &arguments[pipeIndex + 1]);
        perror("couldn't execute command");
        exit(127);
    }else{
        //parent
        close(1);
        dup(fd[1]);
        close(fd[0]);

        if (strcmp(arguments[pipeIndex], "|&") == 0)
        {
            close(2);
            dup(fd[1]);
        }
    }
}