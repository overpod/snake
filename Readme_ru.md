### Как использовать для Windows?

Установите следующие пакеты:
 
 * [Visual Studio Code](https://code.visualstudio.com/docs/?dv=win) 
 * [git](https://git-scm.com/)
 * [msys2](https://www.msys2.org/)
 
Выполните следующие действия:
 
#### Установите MinGW в msys2.
Введите эту команду в msys2 
```
pacman -S mingw-w64-x86_64-toolchain  
```
![avatar](https://github.com/overpod/snake/blob/main/image/Install_MinGW_in_msys2.PNG?raw=true)

#### Создайте новый путь в разделе "Путь".
Способ: Этот компьютер → правая кнопка мыши (нажмите свойства) → дополнительные системные параметры → переменные среды → в окне системные переменные измените "Путь" → добавьте 2 пути:
```
  C:\msys64\mingw64\bin  
```
```
  C:\msys64\usr\bin
```

![avatar](https://github.com/overpod/snake/blob/main/image/path.PNG?raw=true)

#### Установите make в msys2.
Введите эту команду добавить в msys2:
```
  pacman -S make 
```

#### Создание клона каталога raylab.
В msys2 перейдите в каталог:
```
С:\
```  
Введите эту команду: 
```
git clone https://github.com/raysan5/raylib.git 
```
#### Запустите raylib. 
В msys2 перейдите в каталог:

```
 C:\raylib\src  
```
Введите в msys2 эту команду:
```
make 
```
#### Создание клона каталога "Snake".
* Выберите папку, в которой вы хотите работать.
* В msys2, в вашем каталоге.
* Введите эту команду:
```
git clone https://github.com/overpod/snake.git  
```



### Для разработки на Linux

Это руководство предназначено для всех дистрибутивов GNU/Linux, обратите внимание, что тут используется менеджер пакетов APT. APT используется во всех дистрибутвах на основе Debian. 

#### Установите необходимые инструемнты

Вам понадобится GCC (или альтернативный компилятор C99), make и git (для загрузки репозитория raylib).

~~~
sudo apt install build-essential git
~~~

#### Установите необходимые библиотеки

Вам необходимо установить библиотеки; ALSA для аудио, Mesa для ускоренной графики OpenGL и X11 для оконной системмы.

~~~
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
~~~

#### Сборка raylib с помощью make

Мы используем стическую сборку raylib
Загрузите репозиторий raylib с [Github](https://github.com/raysan5/raylib.git), затем скомпилируйте его
```
git clone https://github.com/raysan5/raylib.git raylib
cd raylib/src/
make PLATFORM=PLATFORM_DESKTOP # Соберет статическую версию библиотеки
sudo make instal
```

Если у вас был запущен редактор Visual Studio Code, перезапустите его.
Для запуска сборки проекта необходимо выполнить
~~~
make
~~~

Для запуска заданий из Makefile удобно использовать
[Make support and task provider](https://marketplace.visualstudio.com/items?itemName=carlos-algms.make-task-provider)
