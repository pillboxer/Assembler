#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "error/Error.h"
#include "components/Stripper.h"
#include "components/Parser.h"

#define MAX_ASSEMBLED_SIZE 1048576

static void assemble(const char *file, char *output_name);

// Entry point for program
int main(int argc, char** argv) {
	
	switch (argc) {
		case 2:
			// No file name specified - revert to "assembled.hack"
			assemble(argv[1], NULL);
			break;
		case 3:
			// Use client-specified file name
			assemble(argv[1], argv[2]);
			break;
		default:
			// ❌
			exit_with_error(INCORRECT_ARGUMENT_COUNT);		
	}
	return 0;

}

// Assembles a file into binary code
static void assemble(const char *file_name, char *output_name) {

	// Create a HashMap to store variables and label definitions
	HashMap* hash_map = hash_map_create();

	long size_in_bytes;
	char* file_to_assemble;
	printf("Starting assembly of file %s\n", file_name);
	
	// Check if the file exists
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		exit_with_error(FILE_NOT_FOUND);
	}
	else {
		// Retrieve the size of the file (max 500k)
		fseek(file, 0, SEEK_END);
		size_in_bytes = ftell(file);

		if (size_in_bytes > MAX_ASSEMBLED_SIZE / 2) {
			exit_with_error(FILE_TOO_LARGE);
		}
		
		fseek(file, 0, SEEK_SET); 
		file_to_assemble = malloc(size_in_bytes + 1);

		if (file_to_assemble) {
			
			// Read the contents of the file into the buffer
			fread(file_to_assemble, 1, size_in_bytes, file);
			char no_comments[size_in_bytes + 1];
			char no_spaces[size_in_bytes + 1];
			char no_labels[size_in_bytes + 1];
			char parsed[MAX_ASSEMBLED_SIZE + 1];
			
			// Remove comments and blank spaces. Save and remove labels, save variables
			strip_comments(no_comments, file_to_assemble);
			strip_spaces(no_spaces, no_comments);
			strip_labels(no_labels, no_spaces, hash_map);
			save_variables(no_labels, hash_map);

			// Translate the remaining assembly code to binary
			parse(parsed, no_labels, hash_map);

			char output_file_name[256];

			// If the client chose a custom output name
			if (output_name != NULL) {
				strcpy(output_file_name, output_name);
			}
			else {
				strcpy(output_file_name, "assembled.hack");
			}
			
			// Write the file
			FILE *output = fopen(output_file_name, "w");
			fwrite(parsed, 1, strlen(parsed), output);
			fclose(file);

			printf("Assembly complete");
		}
		else {
			// Could not open file. Bail
			exit_with_error(FILE_READ_ERROR);
		}
	}
}
