/*
Created by: Luke Fanizzi and Peter Haushalter
Description: This program is the main program our shell will be running on. It controls parsing the command
line and executing the functions. The arguments on the comand line are put into a char * so we can access
the arguments in each program.
*/
int no_clobber = 0;
int ready_index, running_index = 0;
#include <unistd.h>  //included files and libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <glob.h>
#include <signal.h>
#include <pthread.h>
#include "sh.h"
#include "thread.h"

pid_t childPid = (int)NULL;
linkedList *listOfUsersHead = NULL;
volatile int caughtSIGCHILD, caughtSIGINT;
/*
	This function is a callback function that is used catch SIGINT.
	Input:
		int signalNumber - The kill signal number.

*/
void childSigHandler(int signalNumber){
	//Warning: Do not make signal handler complex.
	caughtSIGCHILD = 1;
}

void sigHandler(int signalNumber){ 
	//Warning: Do not make signal handler complex.
	caughtSIGINT = 1;
}

void sigChildHandler(int signalNumber){
	//Warning: Do not make signal handler complex.
	caughtSIGCHILD = 1;
}

int main(int argc, char **argv, char **envp){
	char buffer[MAXLINE]; //declarations
	char *arguments[MAXARGS];
	char *ptr;
	char prompt[16];
	int	status, isPiped = OFF, isBlockingProcess = OFF, fd[2];
	int SAVED_STD_OUT = dup(1), SAVED_STD_ERR = dup(2);
	int numberOfThreads = 0;
	pthread_t threadOne;
	pthread_mutex_t mutex;
	
	for(int index = 0; index < MAXARGS; index++){
		prompt[index] = '\0';
	}

	signal(SIGINT, sigHandler);
	signal(SIGCHLD, childSigHandler);

	while(1){
		printf("%s [%s]>> ", prompt, getcwd(NULL, 0));
		fgets(buffer, MAXLINE, stdin);

		//continues if user just enters "\n".
		if (strlen(buffer) == 1 && buffer[strlen(buffer) - 1] == '\n'){
			goto nextprompt;
		}

		if (buffer[strlen(buffer) - 1] == '\n'){
			buffer[strlen(buffer) - 1] = '\0'; 
		}
 

		int argumentIndex = 0; //declarations
		glob_t globPaths;
  		char **globOutput;
        char *token = strtok(buffer, " "); // tokenizes the command line for parsing

        while (token != NULL && argumentIndex < MAXARGS){	 //This loop is responsible for parsing the command line				   
			if (glob(token, 0, NULL, &globPaths) == 0) { // and storing the arguments in a charcter array
				for (globOutput = globPaths.gl_pathv; *globOutput != NULL; globOutput++){
					arguments[argumentIndex] = *globOutput;
					argumentIndex++; //increments index of new array
				}
			}else{
				arguments[argumentIndex] = token; //stores argument
				argumentIndex++; //increments index
			}

			token = strtok (NULL, " "); 
        }

		arguments[argumentIndex] = (char *) NULL; 

		//continues if "blank" command.
		if (arguments[0] == NULL){
			goto nextprompt;
		}

		struct pathelement *path;
		char *cmd;

		int pipeIndex = 0;
		while (arguments[pipeIndex] != NULL){
			if (arguments[pipeIndex][0] == '|'){
				isPiped = ON;
				pipeCommand(arguments, pipeIndex, (int *) fd);
				break;
			}
			pipeIndex++;
		}

		if(!no_clobber){
			for(int i = 0; i<argumentIndex; i++){
				if(!arguments[i+1]){
					break;
				}
				else if (strcmp(arguments[i], ">") == 0 ) {  //redirect output to file
					
					printf("redirecting output ...\n");
					redirectOutput(arguments[0],arguments[i+1]);
					}else if (strcmp(arguments[i], ">&") == 0) { //redirect output and stderr to file
						printf("redirecting output and stderr ...\n");
						redirectOutput2(arguments[0],arguments[i + 1]);
					}else if (strcmp(arguments[i], ">>") == 0) {  //append output to file
						if(!access(arguments[i+1],F_OK)){
							printf("appending output ...\n");
							redirectAppend(arguments[0],arguments[i + 1]);
						}
						else{
							fopen(arguments[i + 1],"a"); //creates file
							fclose((FILE *) arguments[i + 1]);
							printf("appending output (new file) ...\n");
							redirectAppend(arguments[0],arguments[i + 1]);

						}
					}else if (strcmp(arguments[i], ">>&") == 0) {//append output and stderr to file
						if(!access(arguments[i+1],F_OK)){
							printf("appending output and stderr ...\n");
							redirectAppend2(arguments[0],arguments[i + 1]);
						}
						else{
							fopen(arguments[i + 1],"a"); //creates file
							fclose((FILE *) arguments[i + 1]);
							printf("appending output and stderr (new file) ...\n");
							redirectAppend2(arguments[0],arguments[i + 1]);

						}
					}else if (strcmp(arguments[i], "<") == 0) { //redirect input to file
						redirectInput( arguments[0], arguments[i+1]);
					}
			}
		}
		else if(no_clobber){
			for(int i = 0; i<argumentIndex; i++){
				if(!arguments[i+1]){
					break;
				}
				else if (strcmp(arguments[i], ">") == 0 ) {  //redirect output to file
					if(!access(arguments[i+1],F_OK)){
						printf("Cannot overwrite existing files in noclobber mode 1\n");
					}
					else{
						printf("File doesn't exist!\n");
						//redirectOutput(arguments[0],arguments[i+1]);
					}
					}else if (strcmp(arguments[i], ">&") == 0) { //redirect output and stderr to file
						if(!access(arguments[i+1],F_OK)){
						printf("Cannot overwrite existing files in noclobber mode 1\n");
					}else{
						printf("File doesn't exist!\n");
						//redirectOutput2(arguments[0],arguments[i + 1]);
					}
					}else if (strcmp(arguments[i], ">>") == 0) {  //append output to file
						if(!access(arguments[i+1],F_OK)){
							printf("appending output ...\n");
							redirectAppend(arguments[0],arguments[i + 1]);
						}
						else{
							printf("Cannot create new file in noclobber mode 1\n");
						}
					}else if (strcmp(arguments[i], ">>&") == 0) {//append output and stderr to file
						if(!access(arguments[i+1],F_OK)){
							printf("appending output and stderr ...\n");
							redirectAppend2(arguments[0],arguments[i + 1]);
						}
						else{
							printf("Cannot create new file in noclobber mode 1\n");
						}
					}else if (strcmp(arguments[i], "<") == 0) { //redirect input to file
						redirectInput( arguments[0], arguments[i+1]);
					}
			}
		}
		if (strcmp(arguments[0], "exit") == 0) {
			printf("You have exited the shell.\n");
			pthread_cancel(threadOne); 
			pthread_join(threadOne, NULL);

			// for(linkedList *temp = listOfUsersHead; temp != NULL; temp=temp->next){
			// 	printf("%s", temp->name);
			// 	free(temp->name);

			// }
			return 1;
		}else if(strcmp(arguments[argumentIndex-1], "&") == 0){
			//printf("Im here");
			arguments[argumentIndex-1] = NULL;
			//if(strcmp(arguments[argumentIndex][strlen(arguments[argumentIndex - 1])], "&") == 0){
		
			pid_t child = fork();
			if (child == 0)
			{
				struct pathelement *path = get_path();
				char *cmd = which(arguments[0], path);
				//execve(cmd, arguments, NULL);
				//execve(arguments[0], arguments, NULL);
				execvp(arguments[0], arguments);
				perror("Background Process Failed");
			}

		}else if (strcmp(arguments[0], "which") == 0) {
		  	struct pathelement *path, *tmp;
            char *cmd;

			//continues if no agruments are given to to which
		  	if (arguments[1] == NULL) {
		    	printf("which: Too few arguments.\n");
		    	goto nextprompt;
            }
		  	path = get_path();  //gets path
            cmd = which(arguments[1], path);

            if (cmd) {
				printf("%s\n", cmd);
                free(cmd);
            }else{
				printf("%s: Command not found\n", arguments[1]);
			}

		  	while (path) {
		 		tmp = path;
		 		path = path->next;
		 		free(tmp->element);
		 		free(tmp);
            }

		}else if(strcmp(arguments[0], "where") == 0){

			struct pathelement *path, *tmp;
            char **pathFound = NULL;

			//continues if no agruments are given to to which
	  		if (arguments[1] == NULL) {	
				printf("where: Too few arguments.\n");
		    	goto nextprompt;
            }

			path = get_path();

            pathFound = where(arguments[1], path);

            if (pathFound != NULL){
				int index = 0;
				while(pathFound[index] != NULL) {
		 			free(pathFound[index]);
					index++;
            	}
				free(pathFound);  // frees memory
            }else{
				printf("%s: Command not found\n", arguments[1]); // prints if command is not found
			}

			while (path) {
				tmp = path;
		 		path = path->next;
		 		free(tmp->element);
		 		free(tmp); 
            }

		}else if (strcmp(arguments[0], "pwd") == 0) {
			pwd();
	    }else if(strcmp(arguments[0], "pid") == 0){
			printf("%d\n",getpid());
		}else if(strcmp(arguments[0], "kill") == 0){
			killProc(arguments);
		}else if (strcmp(arguments[0], "printenv") == 0) {
			printenv(envp, argumentIndex, arguments);
		}else if (strcmp(arguments[0], "setEnviroment") == 0) {
			setEnvironment(envp, arguments);
		}else if (strcmp(arguments[0], "t_init") == 0) {
			t_init();
		}else if (strcmp(arguments[0], "t_shutdown") == 0) {
			t_shutdown();
		}else if (strcmp(arguments[0], "t_create") == 0) {
			//t_create();
		}else if (strcmp(arguments[0], "t_terminate") == 0) {
			//t_terminate();
		}else if(strcmp(arguments[0], "prompt") == 0){
			if(arguments[1] == NULL){
				printf("Retype a new prompt\n");
			}else if(strcmp(arguments[1], "clear") == 0){
				for(int index = 0; index < MAXARGS; index++){
					prompt[index] = '\0';
				}
			}else{
				strncpy(prompt, arguments[1], MAXARGS);
			}
		}else if (strcmp(arguments[0], "cd") == 0) {
			//I did this because cd function does not like argument[1] being NULL.
			if(arguments[1] == NULL){
				cd("~");
			}else{
				cd(arguments[1]);
			}
		}else if (strcmp(arguments[0], "list") == 0) {
			list(arguments);
		}else if(strcmp(arguments[0], "noclobber") == 0) {
			no_clobber = noclobber(no_clobber);
		}else if(strcmp(arguments[0], "watchuser") == 0){

			//If second argument is null goes onto next terminal prompt.
			if(arguments[1] == NULL){
				printf("No user");
				goto nextprompt;
			}
			
			if(numberOfThreads == 0){
				pthread_create(&threadOne, NULL, watchUser, NULL);
				numberOfThreads = 1;
			}

			if(arguments[2] != NULL && strcmp(arguments[2], "off") == 0){

				pthread_mutex_lock(&mutex);
				for(linkedList *temp = listOfUsersHead; temp != NULL; temp=temp->next){
					if(strcmp(temp->name, arguments[1]) == 0){
						temp->isOn = OFF;
						goto nextprompt;
					}
				}
				pthread_mutex_unlock(&mutex);

			}else{

				pthread_mutex_lock(&mutex);

				for(linkedList *temp = listOfUsersHead; temp != NULL; temp=temp->next){
					if(strcmp(temp->name, arguments[1]) == 0){
						temp->isOn = ON;
						goto nextprompt;
					}
				}

				linkedList *newUser = malloc(sizeof(linkedList));
				newUser->name = malloc(strlen(arguments[1]));
				strcpy(newUser->name, arguments[1]);

				newUser->isOn = ON;
				newUser->next= NULL;

				if(listOfUsersHead == NULL){
					listOfUsersHead = newUser;
				}else{
					linkedList *temp = listOfUsersHead;
					while(temp->next != NULL){
						temp = temp->next;
					}
					temp->next = newUser;

				}

				pthread_mutex_unlock(&mutex);
			}

		}else{
			struct pathelement *path, *tmp;
			char *cmd;

			childPid = fork();


			if(childPid < 0) {
				perror("fork error");
		  	}else if (childPid == 0) {
				//child

				arguments[pipeIndex] = (char *) NULL;

				path = get_path();
				cmd = which(arguments[0], path);
				execve(cmd, arguments, NULL);
				execve(arguments[0], arguments, NULL);
				perror("couldn't execute command");
				exit(EXIT_FAILURE);
			}

			waitpid(0, NULL, 0);
		}

		//must close pipe and initalize standard output and error to terminal.
		//must also be on the bottom as it needs to wait for builtin commands.
		if(isPiped == ON){
			close(fd[1]);
			close(1);
			dup(SAVED_STD_OUT);

			close(2);
			dup(SAVED_STD_ERR);
			isPiped = OFF;

			waitpid(0, NULL, 0);
		}

		if(caughtSIGCHILD == 1){
	 		//Use a non-blocking call (WNOHANG) to be sure this signal handler will not
			//block if a child was cleaned up in another part of the program.
			//This will catch background proceses.
			while (waitpid(0, NULL, WNOHANG) > 0) {
			}
			caughtSIGCHILD = 0;
		}

		if(caughtSIGINT == 1){
			caughtSIGINT = 0;
		}

        nextprompt:
		globfree(&globPaths);

		// for(int index = 0; index < argumentIndex; index++){
		// 	printf("argument[%d]: %s\n", index, arguments[index]);
		// }
	}

	exit(EXIT_SUCCESS); //exits program
}