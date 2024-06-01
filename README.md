# BrickGame.Tetris
Implementation Tetris game in C with console interface (using ncurses library)

## Project description

![Tetris](../misc/images/tetris_view.png) 

The project consist of two parts for implementing the Tetris game: a library that implements the game's logic, which can be connected to various GUIs in the future, and a terminal interface, created using ncurses library. The logic of the library implemented using finite-state machine:

![Tetris finite-state machine](../misc/images/fsm_tetris.png)

### Controls:

Start game - `Enter`;

Pause - `p`;

End game - `q`;

Moving left - `Left arrow`;

Moving right - `Right arrow`;

Falling - `Down arrow`;

Rotation - `Space`.

## Building project

Program library code located in the `src/brick_game/tetris` folder.
Program interface code located in the `src/gui/cli` folder.
Installation directory `src/install`.

Makefile targets: 

`all` - builds the project and launches the game;

`install` - builds the project and places in an installation directory;

`uninstall` - removes the installation directory;

`clean` - removes the object files;

`dvi` - creates documentation;

`dist` - creates distribution package;

`test` - runs tests;

`gcov_report` - generates coverage report;

`tetris.a` - compiles static Tetris library;

`play` - launches the game;

## Project requirements

Insall the ncurses library.

`sudo apt-get install libncurses5-dev`


