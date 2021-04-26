# Top-down Shooter  - my final project for course GameDev C++

![Image alt](https://github.com/Animery/final_project_from_courses/raw/master/sceenshots/screenshot_2.png)

![Image alt](https://github.com/Animery/final_project_from_courses/raw/master/sceenshots/screenshot_3.1.png)



### Features

 * graphic is based on opengl.
 * works on gnu/linux.

SYSTEM REQUIREMENTS
-------------------

Before you enjoy the game make sure that you have a 3D-accelerated video card and latest drivers for your card with 3D-acceleration support.

Currently are supported: gnu/linux.

LINUX
-----

An universal way to play  on linux is to build the program with help of the cmake build system. You probably can install it from the main repository of your linux distribution.
Also your system must have the SDL packages (`SDL`), the same devel packages and gcc.

### NOTE FOR FEDORA USERS

In order to build on Fedora, you need to install the following packages (tested with Fedora 33)

    # dnf install cmake make ninja gcc-c++ SDL2-devel

build On Fedora linux

```sh
$ git clone git@github.com:Animery/home_work.git
$ mkdir build
$ cd build
$ cmake -G Ninja .. 
$ ninja
```

run

```sh
$ ./start
```


Controls
--------

 * `w` - Up
 * `a` - Left
 * `s` - Down
 * `d` - Right
 * `space` - Swap weapon
 * `esc` - Exit
 * `left mouse click` - Fire
