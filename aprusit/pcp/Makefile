all: pcp

CC=gcc
OBJS = list.o
LIBS= -pthread

pcp: list.o
	gcc -o $@.out main.c -Wno-deprecated-declarations ${OBJS} $(LIBS) -std=c99

list.o: ../list/list.h ../list/list.c
	$(CC) -c ../list/list.h ../list/list.c ${LIBS} -std=c99

clean:
	rm -rf *.out *.gch *o pcp
