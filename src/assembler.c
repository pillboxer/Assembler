#include "error/Error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "components/Stripper.h"
#include "components/Parser.h"

#define MAX_ASSEMBLED_SIZE 1048576

void assemble(const char *file, char *output_name);

int main(int argc, char** argv) {
	
	switch (argc) 
	{
		case 2:
			assemble(argv[1], NULL);
			break;
		case 3:
			assemble(argv[1], argv[2]);
			break;
		default:
			exit_with_error(INCORRECT_ARGUMENT_COUNT);		
	}
	return 0;
}

void assemble(const char *file_name, char *output_name) {
	HashMap* hash_map = hash_map_create();
	long size_in_bytes;
	char* file_to_assemble;
	printf("Starting assembly of file %s\n", file_name);
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		exit_with_error(FILE_NOT_FOUND);
	}

	else {
		fseek(file, 0, SEEK_END);
		size_in_bytes = ftell(file);

		if (size_in_bytes > MAX_ASSEMBLED_SIZE / 2) {
			exit_with_error(FILE_TOO_LARGE);
		}

		fseek(file, 0, SEEK_SET); 
		file_to_assemble = malloc(size_in_bytes + 1);
		if (file_to_assemble) {
			fread(file_to_assemble, 1, size_in_bytes, file);
			char no_comments[size_in_bytes + 1];
			char no_spaces[size_in_bytes + 1];
			char no_labels[size_in_bytes + 1];
			char parsed[MAX_ASSEMBLED_SIZE + 1];
			strip_comments(no_comments, file_to_assemble);
			strip_spaces(no_spaces, no_comments);
			strip_labels(no_labels, no_spaces, hash_map);
			save_variables(no_labels, hash_map);
			parse(parsed, no_labels, hash_map);
			char output_file_name[256];
			if (output_name != NULL) {
				strcpy(output_file_name, output_name);
			}
			else {
				strcpy(output_file_name, "assembled.hack");
			}
			FILE *output = fopen(output_file_name, "w");
			fwrite(parsed, 1, strlen(parsed), output);
			fclose(file);
			printf("Assembly complete");
		}
		else {
			exit_with_error(FILE_READ_ERROR);
		}
	}

}
