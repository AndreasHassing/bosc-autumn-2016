all: sqrtsum sqrtsum_single

UNAME := $(shell uname)

# Compiler arguments
CC=gcc
CCFLAGS=-Wall -Werror

# Linker arguments
LDFLAGS=
LDLIBS=-lm

ifneq ($(UNAME), Darwin)
	LDFLAGS=-pthread
endif

sqrtsum: sqrtsum.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@.out sqrtsum.o $(LDLIBS)

sqrtsum.o: sqrtsum.c
	$(CC) $(CCFLAGS) -c sqrtsum.c

sqrtsum_single: sqrtsum_single.o
	$(CC) $(CCFLAGS) -o $@.out sqrtsum_single.o $(LDLIBS)

sqrtsum_single.o: sqrtsum_single.c
	$(CC) $(CCFLAGS) -c sqrtsum_single.c

.PHONY: clean

clean:
	rm -rf *.o *.gch *.out

debug: CCFLAGS += -DDEBUG -g
debug: clean all
