CC = gcc
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`

LDLIBS = `pkg-config --libs sdl2 SDL2_image`


SRC = tools.c main.c
OBJ = ${SRC:.c=.o}

main: tools.o

.PHONY: clean

clean:
	${RM} ${OBJ} main
