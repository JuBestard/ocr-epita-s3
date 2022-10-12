CC = gcc
CPPFLAGS = -lm -MMD
CFLAGS = -Wall -Wextra -std=c99 -O3

SRC = xor.c
DEP = ${SRC:.c=.d}
PRG = ${SRC:.c}

main: xor

-include ${DEP}

clean:
	rm -f *.o ${DEP}
	rm -f ${PRG}

.PHONY: all cleanCC=gcc
