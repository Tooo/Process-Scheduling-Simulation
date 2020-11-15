CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: main.o list.o textmenu.o
	gcc $(CFLAGS) main.o list.o textmenu.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

list.o: list.c
	gcc $(CFLAGS) -c list.c

textmenu.o: textmenu.c
	gcc $(CFLAGS) -c textmenu.c

clean:
	rm a.out *.o