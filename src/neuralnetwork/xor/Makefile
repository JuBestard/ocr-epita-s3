CC = gcc
CPPFLAGS = -lm -MMD
CFLAGS = -Wall -Wextra -std=c99 -O3

SRC = xor.c neuron.c
OBJ = neuron.o xor.o
PRG = xor

all: xor

xor: $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(PRG) $(OBJ) 


clean:
	rm -f *.o ${DEP}
	rm -f ${PRG}

.PHONY: all cleanCC=gcc
