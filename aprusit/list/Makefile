all: fifo

CC=gcc
OBJS = list.o
LIBS= -pthread

fifo: list.o
	gcc -o $@ main.c ${OBJS} $(LIBS) -std=c99

list.o:
	$(CC) -c list.h list.c ${LIBS} 

clean:
	rm -rf *o fifo
