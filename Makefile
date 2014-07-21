OBJS := main.o
BIN  := run

INCPATH=/usr/lib/libdwf.so.2.7.5

all: $(OBJS)
	gcc -static -Llibdwf -o $(BIN) $(OBJS)

main.o: main.c main.h
	gcc -c -std=c99 main.c

clean:
	rm $(OBJS) $(BIN)
