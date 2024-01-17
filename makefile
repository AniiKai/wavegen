
CC = gcc
CFLAGS = -lm

default: main.o
	$(CC) -o out.exec main.o $(CFLAGS) 
	rm *.o

main.o: main.c
	$(CC) -c main.c
