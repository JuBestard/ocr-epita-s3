CC = gcc
CPPFLAGS = -lm -MMD
CFLAGS = -Wall -Wextra -std=c99 -O3

SRC = load_save.c solver.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
PRG = solver

solver: $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(PRG) $(OBJ) 


clean:
	rm -f ${OBJ} ${DEP}
	rm -f ${PRG}

.PHONY: all cleanCC=gcc
