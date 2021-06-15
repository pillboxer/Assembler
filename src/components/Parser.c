#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

#define VALID_DESTINATION_COUNT 7
#define VALID_COMPUTATION_COUNT 28

static bool is_a_command(const char* str);
static bool is_c_command(const char* str);

bool is_valid_destination(const char *str);
const char* valid_destinations[VALID_DESTINATION_COUNT] = { "D", "M", "A", "MD", "AM", "AD", "AMD" };
const char* valid_computations[VALID_COMPUTATION_COUNT] = { "0", "1", "-1", "D", "A", "!D", "!A", "-D", "-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", "A-D", "D&A", "D|A", "M", "!M", "-M", "M+1", "M-1", "D+M", "D-M", "M-D", "D&M", "D|M" };

int num_commands(const char* str) {
	
int count = 0;
	int current_size = 1;
	int current_command_pos = 0;
	char *current_command = malloc(sizeof(char) * current_size);
	printf("Testing string %s\n", str);

	if (current_command == NULL)
		// Change to exit with error
		return 0;
	
	for (int i = 0; i < strlen(str) + 1; i++) {
		if (str[i] == '\n' || str[i] == '\0') {
			printf("\nNEWLINE!\n");
			current_command_pos = 0;
			current_size = 1;
			count++;
			printf("Here's your command %s\n\n", current_command);
			// current_command = realloc(current_command, current_size);
			// check current command
			// Add to count if valid
			// Reset command pos				
		}
		else {
			current_size++;
			printf("Increasing current size by 1. Is now %d\n", current_size);
			
			printf("Adding character %c to position %d\n", str[i], current_command_pos);
			current_command[current_command_pos] = str[i];

			printf("Adding terminator to position %d\n", current_command_pos + 1);
			current_command[current_command_pos + 1] = '\0';

			current_command_pos++;
			current_command = realloc(current_command, current_size);

			// build up the string
		}
	}
	return count;
}

command_type_t command_type(const char* str) {
	if (is_a_command(str)) 
		return ADD;
	if (is_c_command(str))
		return COMP;
	return UNKNOWN;
}

static bool is_a_command(const char* str) {
	return str[0] == '@';
}


static bool is_c_command(const char* str) {
	
	int assignment_operator_position = 0;

	// Check if there is a destination
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '=') {
			assignment_operator_position = i;
		}
	}
	if (assignment_operator_position != 0) {
		// We have some destination
		printf("pos is %d\n", assignment_operator_position);
		// Create a string with the length of the string up to the '=', add 1 for null terminator 
		char *destination = malloc(sizeof(char) * assignment_operator_position + 1);
		strncpy(destination, str, assignment_operator_position);
		destination[assignment_operator_position] = '\0';
		if (!is_valid_destination(destination)) {
			printf("Invalid destination %s\n", destination);
			return false;
		}
		
	}

	return false;

}

bool is_valid_destination(const char* str) {
	for (int i = 0; i < VALID_DESTINATION_COUNT; i++) {
		if (str == valid_destinations[i]) {
			return true;
		}
	}
	return false;
}



