OBJS := main.o
BIN  := run

all: $(OBJS)
	gcc -o $(BIN) $(OBJS) -L/usr/lib -ldwf

main.o: main.c main.h
	gcc -c -std=c99 main.c

clean:
	rm $(OBJS) $(BIN)
