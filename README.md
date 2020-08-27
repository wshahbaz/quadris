# QUADRIS
The game of Quadris is played on a 15x11 (row x column) using tetrominoes, similarlry to Tetris, except blocks do not fall down automatically (ie the game is static). 3 additional rows are at the top of the board to give room for the initial positioned piece to rotate and move.

This project is concerned with implementing this game in C++ using GTKmm library for the UI and Makefiles to manage the build tree.

There are 5 levels that the user can toggle between while playing the game:
- Level 0: blocks are generated via a `sequence.txt` file
- Level 1: blocks are generated randomly, with nicer weightings
- Level 2: blocks are generated randomly, with harsher weightings
- Level 3: blocks are generated randomly and drop 1 block after any user move
- Level 4: all rules of level 3, plus random blocks are generated every few turns and automatically placed
- Level 5: similar to level 2, with a twist of 3 additional, complex shaped blocks has been introduced

### Commands:
The user has the following text inputs to enter into the command line to play:
- left, right, down: move the current block by 1 block shift
- clockwise/counterclockwise: rotate the current block
- drop: places the current block in the lowest position, ending the turn
- levelup/leveldown: increment/decrement the current level
- restart: restarts the current game
- rename \[previous command name\] \[new command name\]
The command interpreter accepts unique prefixes of any command, and multipliers at the beginning of the command name.

### Command Arguments
- -seed xxx: set a random seed so the random levels will act 'random'
- -text: run the program using only the text-based ui

### Scoring:
Points are earned from removing blocks from the board via row eliminations, speicifically, points are earned for row eliminations, and extra points are earned when all components of a tetronimo is eliminated. The higher the level, the higher the rewards.

## Getting Started
Ensure that you have the necessary dependencies installed. At the minimum the latest version of GTKmm and g++ should be installed. To compile the project, execute the command `make`, followed by `./quadris [any command line arguments]` to execute the app.

