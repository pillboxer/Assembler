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
	
	switch (code) {
		case INCORRECT_ARGUMENT_COUNT:
			*str = "Incorrect argument count";
			break;
		case FILE_NOT_FOUND:
			*str = "The file doesn't exist";
			break;
		case FILE_READ_ERROR:
			*str = "Could not read file";
			break;
		case NULL_POINTER:
			*str = "Null pointer found";
			break; 
		case UNKNOWN_COMMAND:
			*str = "Unrecognized command";
			break;
		case MALFORMED_DESTINATION:
			*str = "Invalid destination received";
			break;
		case MALFORMED_COMPUTATION:
			*str = "Received a missing computation";
			break;
		case MALFORMED_JUMP:
			*str = "Invalid jump received";
			break;
		case FILE_TOO_LARGE:
			*str = "File too large, please ensure it is less than 500kb";
			break;
		default:
			*str = "Unknown error.";
		break;
	}
}
