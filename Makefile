# Define raylib source code path
RAYLIB_SRC_PATH ?= c:\raylib\src
# Define default C compiler
CC ?= gcc

win:
	$(CC) ./main.c -o snake.exe -O2 -Wall -Wno-missing-braces -I c:\raylib\src -L c:\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
	./snake.exe
	rm ./snake.exe
linux:
	$(CC) ./main.c -o snake -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./snake
	rm ./snake

