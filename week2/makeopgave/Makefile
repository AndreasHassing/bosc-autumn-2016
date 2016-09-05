CC=gcc
OUTDIR=out
CFLAGS=-Wall -Werror

# Exercise 2.1
all: opg1

opg1: data.o main.o io.o
	$(CC) $(CFLAGS) data.o io.o main.o -o $(OUTDIR)/opg1

main.o: main.c io.h data.h
	$(CC) $(CFLAGS) -c main.c person.h

data.o: data.c data.h
	$(CC) $(CFLAGS) -c data.c data.h person.h

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c io.h person.h

# Exercise 2.2
.PHONY: clean

clean:
	rm -rf *.o *.gch
