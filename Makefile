# Define raylib source code path
RAYLIB_SRC_PATH ?= c:\raylib\src


ifeq ($(OS),Windows_NT)
    RAYLIB_SRC_PATH = c:\raylib\src
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        RAYLIB_SRC_PATH = $(HOME)/libs/raylib/src
    endif
endif

# Define default C compiler
CC ?= gcc

win:
	$(CC) ./main.c -o snake.exe -O2 -Wall -Wno-missing-braces -I c:\raylib\src -L c:\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
	./snake.exe
	rm ./snake.exe
linux:
	$(CC) ./main.c -o snake -O2 -Wall -Wno-missing-braces -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	./snake
	rm ./snake
mac:
	$(CC) main.c -o snake -O2 -Wall -Wno-missing-braces -I$(RAYLIB_SRC_PATH) $(RAYLIB_SRC_PATH)/libraylib.a -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	./snake
	rm ./snake	


