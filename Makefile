# choose your compiler
CC=gcc
# CC=gcc -Wall

mysh: threadLibrary.o
	$(CC) -g T1.c threadLibrary.c -o T1

threadLibrary.o : threadLibrary.c
	$(CC) -g -c threadLibrary.c

clean:
	rm *.o mysh
