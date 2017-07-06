all: banker

UNAME := $(shell uname)

# Compiler arguments
CC=gcc
CCFLAGS=-Wall -Werror

# Linker arguments
LDFLAGS=

ifneq ($(UNAME), Darwin)
	LDFLAGS=-pthread
endif

banker: banker.o array_helpers.o
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@.out banker.o array_helpers.o

banker.o: banker.c
	$(CC) $(CCFLAGS) -c banker.c

array_helpers.o: array_helpers.h array_helpers.c
	$(CC) $(CCFLAGS) -c array_helpers.h array_helpers.c

.PHONY: clean

clean:
	rm -rf *.o *.gch *.out

debug: CCFLAGS += -DDEBUG -g
debug: clean all
