CCFLAGS=

all: virtmem

debug: CCFLAGS += -DDEBUG -g
debug: virtmem

stats: CCFLAGS += -DSTATS
stats: virtmem

virtmem: main.o page_table.o disk.o program.o
	gcc $(CCFLAGS) main.o page_table.o disk.o program.o -o virtmem

main.o: main.c
	gcc $(CCFLAGS) -Wall -g -c main.c -o main.o

page_table.o: page_table.c
	gcc $(CCFLAGS) -Wall -g -c page_table.c -o page_table.o

disk.o: disk.c
	gcc $(CCFLAGS) -Wall -g -c disk.c -o disk.o

program.o: program.c
	gcc $(CCFLAGS) -Wall -g -c program.c -o program.o

.PHONY: clean

clean:
	rm -f *.o virtmem
