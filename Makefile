CC = gcc -g -Iinclude/
CFLAGS = -Wall -Wextra -O3 -D__NO_INLINE__ `pkg-config --cflags sdl2 SDL2_image` -fsanitize=address
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl


SRCNET = src/neuralnetwork/xor/neuron.c src/neuralnetwork/xor/xor.c
OBJNET = ${SRCNET:.c=.o}
DEPNET = ${SRCNET:.c=.d}
PRGNET = neuralnet


SRCAPP = $(wildcard src/app/image_process/*/*.c) $(wildcard src/app/detection_grid/*/*.c) src/app/ocr.c
OBJAPP = ${SRCAPP:.c=.o}
DEPAPP = ${SRCAPP:.c=.d}
PRGAPP = ocr


SRCSOL = $(wildcard src/app/image_process/toolbox/*.c) $(wildcard src/app/solver/*.c) 
OBJSOL = ${SRCSOL:.c=.o}
DEPSOL = ${SRCSOL:.c=.d}
PRGSOL = solver


all: ${PRGAPP} ${PRGNET} ${PRGSOL}

${PRGNET}: ${OBJNET}
	$(CC)  -o $(PRGNET) $(OBJNET) -lm -ldl

${PRGAPP}: ${OBJAPP}
	$(CC) $(CFLAGS) -o $(PRGAPP) $(OBJAPP) $(LDLIBS)

${PRGSOL}: ${OBJSOL}
	$(CC) $(CFLAGS) -o $(PRGSOL) $(OBJSOL) $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJAPP} ${OBJNET} ${OBJSOL}
	${RM} ${DEPAPP} ${DEPNET} ${DEPSOL}
	${RM} ${PRGAPP} ${PRGNET} ${PRGSOL}
	rm -rf out/ given_grid/*.result
