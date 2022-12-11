CC = gcc -g -I include/
CFLAGS = -Wall -Wextra -O2 `pkg-config --cflags sdl2 SDL2_image gtk+-3.0`
LDLIBS = `pkg-config --libs sdl2 SDL2_image gtk+-3.0` -lm -ldl


SRCNET = src/neuralnetwork/nn/training_set/main.c
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
	$(CC)  $(CFLAGS) -o $(PRGNET) $(OBJNET) $(LDLIBS)

#${PRGSOL}: ${OBJSOL}
#	$(CC) $(CFLAGS) -o $(PRGSOL) $(OBJNET) $(LDLIBS)

${PRGAPP}: ${OBJAPP}
	$(CC) $(CFLAGS) -o $(PRGAPP) $(OBJAPP) $(LDLIBS)


.PHONY: clean

clean:
	${RM} ${OBJAPP} ${OBJNET} ${OBJSOL}
	${RM} ${DEPAPP} ${DEPNET} ${DEPSOL}
	${RM} ${PRGAPP} ${PRGNET} ${PRGSOL}
	rm -rf *.png out/ given_grid/*.result
