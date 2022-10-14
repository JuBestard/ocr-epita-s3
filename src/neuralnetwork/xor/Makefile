CC = gcc
CPPFLAGS = -lm -MMD
CFLAGS = -Wall -Wextra -std=c99 -O3

SRC = xor.c neuron.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
PRG = xor

all: xor

xor: $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(PRG) $(OBJ) 


clean:
	rm -f ${OBJ} ${DEP}
	rm -f ${PRG}

.PHONY: all cleanCC=gcc
