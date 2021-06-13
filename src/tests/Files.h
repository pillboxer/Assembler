#include "../components/Parser.h"

const char* ADD_ASM_SRC = "// This file is part of www.nand2tetris.org\n// and the book \"The Elements of Computing Systems\"\n// by Nisan and Schocken, MIT Press.\n// File name: projects/06/add/Add.asm\n\n// Computes R0 = 2 + 3  (R0 refers to RAM[0])\n\n@2\nD=A\n@3\nD=D+A\n@0\nM=D\n";

const char* ADD_ASM_STRIPPED = "@2\nD=A\n@3\nD=D+A\n@0\nM=D";

const command_type_t ADD_ASM_CMDS[6] = { ADD, COMP, ADD, COMP, ADD, COMP }; 
