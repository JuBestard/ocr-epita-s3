all:
	gcc -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image` -c -o main.o main.c -lm
	gcc main.o `pkg-config --libs sdl2 SDL2_image` -o main -lm

clean:
	rm *.o main
