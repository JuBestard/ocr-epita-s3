# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS =

SRC = solver.c 
OBJ = ${SRC:.c=.o}
PRG = ${SRC:.c=}

all: ${PRG}

-include ${DEP}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${PRG}

# END

