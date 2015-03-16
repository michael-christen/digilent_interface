OBJS := main.o impl.o
BIN  := run

all: $(OBJS)
	gcc -o $(BIN) $(OBJS) -L/usr/lib -ldwf

main.o: main.c main.h
	gcc -c -std=c99 main.c

impl.o: impl.c main.h
	gcc -c -std=c99 impl.c

clean:
	rm $(OBJS) $(BIN)
