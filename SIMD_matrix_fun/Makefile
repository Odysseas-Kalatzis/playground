DEBUG_FLAGS=-fsanitize=address -fsanitize=undefined -g -fno-omit-frame-pointer -Wall -Wextra -pedantic -mavx2
CFLAGS=-Wall -Wextra -pedantic -O3 -mavx2
LFLAGS=-lpthread

all:
	gcc matrix.c main.c ${CFLAGS} ${LFLAGS} -o main 

sane:
	gcc matrix.c main.c ${DEBUG_FLAGS} ${LFLAGS} -o main 

clean:
	rm -f main

