all: bosh

OBJS=bosh.o parser.o redirect.o
LIBS=-lreadline -ltermcap
CC=gcc
CCFLAGS=-Wall -Werror

bosh: $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $(OBJS) $(LIBS)

bosh.o: bosh.c
	$(CC) $(CCFLAGS) -c bosh.c parser.h

parser.o: parser.h parser.c
	$(CC) $(CFLAGS) -c parser.h parser.c

redirect.o: redirect.h redirect.c
	$(CC) $(CFLAGS) -c redirect.c

.PHONY: clean

clean:
	rm -rf *.o *.gch bosh

debug: CCFLAGS += -DDEBUG -g
debug: clean bosh
