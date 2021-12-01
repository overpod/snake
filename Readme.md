### How to use for windows? 
 
Install the following packages: 
 
 * [Visual Studio Code](https://code.visualstudio.com/docs/?dv=win) 
 * [git](https://git-scm.com/) 
 * [msys2](https://www.msys2.org/)
 
Do the following: 
 
**1. Install MinGW in msys2.**
Enter this command in msys2:  
```
pacman -S mingw-w64-make  
```
![avatar](https://avatars.mds.yandex.net/get-images-cbir/2191032/pPn3CDdNUvyVflPYbLLmKg1902/ocr)

**2. Create a new path in "Path".** 
Way: This computer → right mouse button (click properties) → additional system parameters → environment variables → in the system variables window, change "Path" → add 2 paths: 
```
  C:\msys64\mingw64\bin  
```
```
  C:\msys64\usr\bin
```

![avatar](/image/path.png)

**3. Install make in msys2.** 
Enter this command add in msys2:
```
  pacman -S make 
```
**4. Creating a clone of the raylab directory.**
In msys2, go to the directory: 
```
С:\
```  
Enter this command:  
```
git clone https://github.com/raysan5/raylib.git 
```
**5. Start raylib.**
In msys2, go to the directory: 

```
 C:\raylib\src.   
```
Enter in msys2 this command:
```
make 
```
**6. Creating a clone of the "Snake Game" directory.**
* Select a folder you want to work in. 
* In msys2, in your directory.  
* Enter this command:  
```
git clone https://github.com/overpod/snake.git  
```
## Good luck

Details for the development of the project: 677VyVGPQxEsdVsnbqcvQh9RvXFYM52qDa1cXpx7bLWS (SOL)