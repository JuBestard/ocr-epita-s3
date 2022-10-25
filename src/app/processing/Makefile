CC = gcc -I../../../include/
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`

LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl


SRC = ../pixels_op/pixels_op.c otsu.c blur.c tools.c grayscale.c main.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ} 
	${RM} main 
	${RM} *.bmp *.png
	${RM} ${DEP}
