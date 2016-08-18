CC=gcc
ODIR=out
CFLAGS=-Wall -Werror -I.

default: clean ex1.1 ex1.2 ex2.1 ex3.1

ex1.1: ex1.1_hello_world.c
	$(CC) -o $(ODIR)/ex1.1_hello_world ex1.1_hello_world.c $(CFLAGS)

ex1.2: ex1.2_args.c
	$(CC) -o $(ODIR)/ex1.2_args ex1.2_args.c $(CFLAGS)

ex2.1: ex2.1_grep.c
	$(CC) -o $(ODIR)/ex2.1_grep ex2.1_grep.c $(CFLAGS)

ex3.1: ex3.1_observer.c
	$(CC) -o $(ODIR)/ex3.1_observer ex3.1_observer.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*
