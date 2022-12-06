CC = gcc -g -I include/
CFLAGS = -Wall -Wextra -O2 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl


SRCNET = src/neuralnetwork/xor/neuron.c src/neuralnetwork/xor/xor.c
OBJNET = ${SRCNET:.c=.o}
DEPNET = ${SRCNET:.c=.d}
PRGNET = neuralnet


SRCAPP = $(wildcard src/app/*/*/*.c) $(wildcard src/app/solver/*.c) src/ocr.c
OBJAPP = ${SRCAPP:.c=.o}
DEPAPP = ${SRCAPP:.c=.d}
PRGAPP = ocr

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
	rm -rf out/ given_grid/*.result
