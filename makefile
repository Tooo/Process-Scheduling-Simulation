CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: main.o list.o textmenu.o process.o message.o semaphore.o
	gcc $(CFLAGS) main.o list.o textmenu.o process.o message.o semaphore.o

main.o: main.c
	gcc $(CFLAGS) -c main.c

list.o: list.c
	gcc $(CFLAGS) -c list.c

textmenu.o: textmenu.c
	gcc $(CFLAGS) -c textmenu.c

process.o: process.c
	gcc $(CFLAGS) -c process.c

message.o: message.c
	gcc $(CFLAGS) -c message.c

semaphore.o: semaphore.c
	gcc $(CFLAGS) -c semaphore.c

clean:
	rm a.out *.o