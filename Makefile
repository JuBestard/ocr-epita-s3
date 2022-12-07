CC = gcc -g -I include/
CFLAGS = -Wall -Wextra -O2 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl


SRCNET = src/neuralnetwork/xor/neuron.c src/neuralnetwork/xor/xor.c
OBJNET = ${SRCNET:.c=.o}
DEPNET = ${SRCNET:.c=.d}
PRGNET = neuralnet

#SRCSOL = $(wildcard src/app/solver/*.c)
#OBJSOL = ${SRCSOL:.c=.o}
#DEPSOL = ${SRCSOL:.c=.d}
#PRGSOL = solver

SRCAPP = $(wildcard src/app/*/*/*.c) $(wildcard src/app/solver/*.c) src/ocr.c
OBJAPP = ${SRCAPP:.c=.o}
DEPAPP = ${SRCAPP:.c=.d}
PRGAPP = ocr

all: ${PRGAPP} ${PRGNET} ${PRGSOL}

${PRGNET}: ${OBJNET}
	$(CC)  -o $(PRGNET) $(OBJNET) -lm -ldl

#${PRGSOL}: ${OBJSOL}
#	$(CC) $(CFLAGS) -o $(PRGSOL) $(OBJNET) $(LDLIBS)

${PRGAPP}: ${OBJAPP}
	$(CC) $(CFLAGS) -o $(PRGAPP) $(OBJAPP) $(LDLIBS)


.PHONY: clean

clean:
	${RM} ${OBJAPP} ${OBJNET} ${OBJSOL}
	${RM} ${DEPAPP} ${DEPNET} ${DEPSOL}
	${RM} ${PRGAPP} ${PRGNET} ${PRGSOL}
	rm -rf out/ given_grid/*.result
