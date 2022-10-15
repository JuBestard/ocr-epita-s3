# Makefile

CC = gcc
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra

SRC = solver.c
OBJ = ${SRC:.c=.o}
PRG = ${SRC:.c=}

all: ${PRG}

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${PRG}

# END

