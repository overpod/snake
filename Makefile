win:
	gcc ./main.c -o main.exe -O2 -Wall -Wno-missing-braces -I c:\raylib\src -L c:\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm
	./main.exe