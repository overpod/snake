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



