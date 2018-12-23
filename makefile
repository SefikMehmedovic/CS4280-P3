#makefile for P2

all: *.c
	gcc -std=c99 -g -Wall -o statSem  *.c

clean:
	$(RM) statSem 
