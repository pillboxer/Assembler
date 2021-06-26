#include "error/Error.h"
#include <stdio.h>

void assemble(const char *file);

int main(int argc, char** argv) {
	
	switch (argc) 
	{
		case 2:
			assemble(argv[1]);
			break;
		default:
			exit_with_error(INCORRECT_ARGUMENT_COUNT);		
	}
	return 0;
}

void assemble(const char *file_name) {
	printf("Starting assembly of file %s\n", file_name);
	FILE *file = fopen(file_name, "r");

	if (file == NULL) {
		exit_with_error(FILE_NOT_FOUND);
	}

	else {
		printf("File found!\n");
	}

}
