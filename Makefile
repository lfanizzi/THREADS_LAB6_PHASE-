# choose your compiler
CC=gcc
# CC=gcc -Wall

mysh: *.c *.o
	$(CC) -g *.o -o mysh

*.o: *.c
	$(CC) -g -c *.c

clean:
	rm *.o mysh
