CC=gcc
CFLAGS=-Wall -Werror

all: exercise1 exercise2

exercise1: forback.o testforback.c
	$(CC) $(CFLAGS) forback.o testforback.c -o testforback.out

forback.o: forback.h forback.c
	$(CC) $(CFLAGS) -c forback.c forback.h

exercise2: redirect.o testredirect.c
	$(CC) $(CFLAGS) redirect.o testredirect.c -o testredirect.out

redirect.o: redirect.h redirect.c
	$(CC) $(CFLAGS) -c redirect.h redirect.c

.PHONE: clean

clean:
	rm -rf *.out *.o *.gch
