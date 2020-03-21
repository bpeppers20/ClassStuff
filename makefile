CC = gcc
CFLAGS = -c -g 

all: master bin_adder
master: master.o
	$(CC) -o master master.o
bin_adder: bin_adder.o
	$(CC) -o bin_adder bin_adder.o -lpthread -lm
master.o: master.c
	$(CC) $(CFLAGS) master.c
bin_adder.o: bin_adder.c
	$(CC) $(CFLAGS) bin_adder.c
clean:
	rm -f *.o master bin_adder
