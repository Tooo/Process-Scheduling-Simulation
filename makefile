CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: main.o list.o
	gcc $(CFLAGS) main.o list.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

list.o: list.c
	gcc $(CFLAGS) -c list.c

clean:
	rm pcb *.o