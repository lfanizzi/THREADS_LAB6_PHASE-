# choose your compiler
CC=gcc
# CC=gcc -Wall

mysh: threadLibrary.o
	$(CC) -g T1.c threadLibrary.c -o T1
	$(CC) -g T1x.c threadLibrary.c -o T1x

threadLibrary.o : threadLibrary.c
	$(CC) -g -c threadLibrary.c

clean:
	rm *.o mysh
