all:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2` -c -o decompose.o decompose.c
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2` -c -o main.o main.c
	gcc decompose.o main.o `pkg-config --libs sdl2` -lm -o decompose

clean:
	rm *.o
	rm decompose

