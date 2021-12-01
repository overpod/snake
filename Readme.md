

### How to use for windows

//**Install the following packages**

1. [Visual Studio Code](https://code.visualstudio.com/docs/?dv=win)
2. [msys2](www.msys2.org)
3. [Install MinGW in msys2. Enter this command: **pacman -S mingw-w64-make** in msys2]
4. [git](https://git-scm.com/)
5. Create a new path in "Path". Way: This computer → right mouse button (click properties) → additional system parameters → environment variables → in the system variables window, change "PACH" → add 2 paths:
* [C:\msys64\mingw64\bin]
* [C:\msys64\usr\bin] 
6. [Install make in msys2. Enter this command: **add pacman -S make** in msys2] 
7. [Creating a clone of the raylab directory.]
* In msys2, go to the directory: **С:\** 
* Enter this commnd: **git clone https://github.com/raysan5/raylib.git**]
8. In msys2, go to the directory: **C:\raylib\src**. Enter in msys2 this commnd: **make**
9. [Creating a clone of the "Snake Game" directory. Select a folder do you want to work]
* In msys2, in the your directory: 
* Enter this commnd: **git clone https://github.com/overpod/snake.git**]

## Good luck