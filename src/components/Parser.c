#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
void commands(command_type_t **ptr, const char *str) {

	command_type_t *copy = malloc(sizeof(*ptr));
	copy[0] = ADD;
	copy[1] = COMP;
	copy[2] = LBL;
	copy[3] = ADD;
	copy[4] = ADD;
	copy[5] = COMP;
	*ptr = copy;	
}
