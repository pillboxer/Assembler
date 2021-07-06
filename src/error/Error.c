#include <stdio.h>
#include <stdlib.h>
#include "Error.h"

void error_string_for_code(int code, char **star);

void exit_with_error(int code) {
	char *str = "";
	error_string_for_code(code, &str);
	printf("Exiting with error: '%s'\n", str);
	exit(code);
}

void error_string_for_code(int code, char **str) {
	
	// Why are we passing a double pointer here?
	// If we pass a single pointer, we are copying the string and placing it on the stack
	// That means, when we manipulate it, the actual parameter does not change
	// If we pass a double pointer however, we then have access to the address
	// of where the value we want to change is
	
	switch (code) 
	{
		case INCORRECT_ARGUMENT_COUNT:
			*str = "Incorrect argument count";
			break;
		case FILE_NOT_FOUND:
			*str = "The file doesn't exist";
			break;
		case NULL_POINTER:
			*str = "Null pointer found";
			break; 
		default:
			*str = "Unknown error.";
		break;
	}

	
}
