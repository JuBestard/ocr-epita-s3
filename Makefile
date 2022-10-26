CC = gcc -Iinclude/
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`

LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl

APPDIR = "src/app/image_process/"
NETDIR = "src/neural_network/xor"

SRCNET = src/neuralnetwork/xor/neuron.c src/neuralnetwork/xor/xor.c
OBJNET = ${SRCNET:.c=.o}
DEPNET = ${SRCNET:.c=.d}

SRCAPP = $(wildcard src/app/image_process/color_treatement/*.c) $(wildcard src/app/image_process/rotation_scale/*.c) $(wildcard src/app/image_process/toolbox/*.c) src/app/image_process/ocr.c
OBJAPP = ${SRCAPP:.c=.o}
DEPAPP = ${SRCAPP:.c=.d}

PRGAPP = ocr
PRGNET = neuralnet

all: ${PRGAPP} ${PRGNET}

${PRGNET}: ${OBJNET}
	$(CC)  -o $(PRGNET) $(OBJNET) -lm -ldl

${PRGAPP}: ${OBJAPP}
	$(CC) $(CFLAGS) -o $(PRGAPP) $(OBJAPP) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJAPP} ${OBJNET}
	${RM} ${DEPAPP} ${DEPNET}
	${RM} ${PRGAPP} ${PRGNET}
	${RM} *.bmp
