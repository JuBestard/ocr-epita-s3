CC = gcc -I../../../include/image_process
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`

LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl


#SRC = tools.c pixels_op/pixels_op.c processing/blur.c processing/otsu.c processing/grayscale.c rotation_scale/scale.c main.c
SRC = $(wildcard color_treatement/*.c) $(wildcard rotation_scale/*.c) $(wildcard toolbox/*.c)
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${DEP}
	${RM} main 
	${RM} *.bmp
