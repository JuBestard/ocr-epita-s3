CC = gcc -I../../../include/
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`

LDLIBS = `pkg-config --libs sdl2 SDL2_image`


SRC = ../pixels_op/pixels_op.c otsu.c blur.c tools.c grayscale.c binarization.c main.c
OBJ = ${SRC:.c=.o}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ} main *.bmp
