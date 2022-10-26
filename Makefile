CC = gcc -Iinclude/
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm -ldl


SRCNET = src/neuralnetwork/xor/neuron.c src/neuralnetwork/xor/xor.c
OBJNET = ${SRCNET:.c=.o}
DEPNET = ${SRCNET:.c=.d}
PRGNET = neuralnet


SRCAPP = $(wildcard src/app/image_process/color_treatement/*.c) $(wildcard src/app/image_process/rotation_scale/*.c) $(wildcard src/app/image_process/toolbox/*.c) src/app/image_process/ocr.c
OBJAPP = ${SRCAPP:.c=.o}
DEPAPP = ${SRCAPP:.c=.d}
PRGAPP = ocr


SRCSOL = src/app/solver/load_save.c src/app/solver/solver.c
OBJSOL = ${SRCSOL:.c=.o}
DEPSOL = ${SRCSOL:.c=.d}
PRGSOL = solver


all: ${PRGAPP} ${PRGNET} ${PRGSOL}

${PRGNET}: ${OBJNET}
	$(CC)  -o $(PRGNET) $(OBJNET) -lm -ldl

${PRGAPP}: ${OBJAPP}
	$(CC) $(CFLAGS) -o $(PRGAPP) $(OBJAPP) $(LDLIBS)

${PRGSOL}: ${OBJSOL}
	$(CC) $(CFLAGS) -o $(PRGSOL) $(OBJSOL)

.PHONY: clean

clean:
	${RM} ${OBJAPP} ${OBJNET} ${OBJSOL}
	${RM} ${DEPAPP} ${DEPNET} ${DEPSOL}
	${RM} ${PRGAPP} ${PRGNET} ${PRGSOL}
	${RM} *.bmp *.result
