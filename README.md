# CppND: Capstone Snake Game 

The code is based on original baseline [Udacity Capstone Snake Game project](https://github.com/udacity/CppND-Capstone-Snake-Game).


<p align="center">
<img src="Capstone.gif"/>
</p>


## Code Structure

<p align="center">
<img src="Code_structure.png"/>
</p>

In this project, one snake is self navigated using search algorithm, expansion method and other snake is controlled by player. If player's snake hits walls or computer snake, game ends there.

## Rubic Points

* C++ functions and structures are demonstrated.  
* Advanced OOP techniques are used. 
* Classes are accessed using access specifiers.  
** members are specified as public and protected in class snake [snake.h], members are specified as public and private in class snake [comp_snake.h].
* Member initialization lists is used.    
* Inheritance hierarchy is incorporated.  
** Class Comp_snake inheritate from class snake
* Virtual base class functions are ovveridden by derived class functions.  
* Move semantics is used to move data, whenever possible.  
* Multithreading is used.   
* A mutex or lock is used.  


## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source.
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
