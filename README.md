# Assembler

This program implements the Assembler as required in [project 6 of the Nand2Tetris course](https://www.nand2tetris.org/course). 

I wrote it in C to improve my knowledge of memory allocation/pointers and to get some hands on in implementing a hash table from scratch. 

## Usage

The Assembler assembles code written in the Hack machine language. You can find specifications of the language [here](https://zhangruochi.com/Machine-Language/2019/09/22/#the-hack-language-specification) and [here](http://lemmingapex.github.io/ElementsOfComputingSystems/projects/project04/chapter%2004.pdf)

The assembly process involves stripping a source file of comments and extraneous whitespace, as well as saving user defined variables and label definitions. It also recognizes usage of reserved variables such as `KBD` and `SCREEN`

### Assembling your file

1.) Compile the Assembler using `make`
2.) With your source code ready, run the program:

`./assembler your_folder/your_source_code.asm my_assembled_file.hack`

Note the output argument (`my_assembled_file.hack`) is optional. If omitted, the outputted file will be named `assembled.hack`

If your source code contains errors, the program will exit with an error

## Test Suite

Project 6 of the Nand2Tetris course provides four valid asm source code files. These are:

1.) ADD   // Simple addition of two integers
2.) RECT  // Draws a rectangle at the top of the screen
3.) MAX   // Computes maximum of two integers
4.) PONG  // The game pong (yes, really)

The valid binary of each file was obtained by assembling it with the supplied Nand2Tetris CPU emulator.

The Assembler has a test suite that compares it's assembled output with the valid binary above. You can run for each program with the following:

1.) ADD   // make tests
2.) RECT  // make tests_rect
3.) MAX   // make tests_max
4.) PONG  // max tests_pong

