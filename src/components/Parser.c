#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include <ctype.h>

#define VALID_DESTINATION_COUNT 7
#define VALID_COMPUTATION_COUNT 28
#define VALID_JUMP_COUNT 7
#define WORD_LENGTH 16

static command_type_t command_type(const char* str);
static bool is_a_command(const char* str);
static bool is_c_command(const char* str);
static bool is_label_definition(const char* str);

static bool is_valid_destination(const char* str);
static bool is_valid_computation(const char* str);
static bool is_valid_jump(const char* str);
static bool is_valid(const char* str, const char** argv, unsigned int count);

const char* valid_destinations[VALID_DESTINATION_COUNT] = { "D", "M", "A", "MD", "AM", "AD", "AMD" };
const char* valid_computations[VALID_COMPUTATION_COUNT] = { "0", "1", "-1", "D", "A", "!D", "!A", "-D", 
															"-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", 
															"A-D", "D&A", "D|A", "M", "!M", "-M", "M+1", 
															"M-1", "D+M", "D-M", "M-D", "D&M", "D|M" };
const char* valid_jumps[VALID_JUMP_COUNT] = { "JGT", "JGE", "JEQ", "JLT", "JLE", "JMP", "JNE" };

// Should ultimately be static
const char* parsed_a_command(const char* cmd);

const char* parsed_a_command(const char* cmd) {
	char *parsed = malloc(sizeof(char) * WORD_LENGTH + 1);
	char lowered[strlen(cmd) + 1];

	for (size_t i = 0; i < strlen(cmd); i++) 
		lowered[i] = tolower(cmd[i]);

	printf("Attempting to parse %s\n", lowered);
	lowered[strlen(cmd)] = '\0';
	parsed[0] = '\0';

	if (lowered[1] == 'r') {
		printf("Dealing with RX\n");
	}

	// Need to deal with more cases

	return parsed;

}

int num_commands(const char* str) {	
	int count = 0;
	int current_size = 1;
	int current_command_pos = 0;
	char *current_command = malloc(sizeof(char) * current_size);

	if (current_command == NULL)
		// Change to exit with error
		return 0;
	
	for (int i = 0; i < strlen(str) + 1; i++) {
		if (str[i] == '\n' || str[i] == '\0') {
			current_command_pos = 0;
			current_size = 1;					
			if (command_type(current_command) == LBL) {
				current_command = realloc(current_command, current_size);
				continue;
			}
			count++;
			if (command_type(current_command) == UNKNOWN)
				return 0;
			current_command = realloc(current_command, current_size);
		}
		else {
			current_size++;
			current_command[current_command_pos] = str[i];
			current_command[current_command_pos + 1] = '\0';
			current_command_pos++;
			current_command = realloc(current_command, current_size);
		}
	}

	free(current_command);
	return count;
}

static command_type_t command_type(const char* str) {
	if (is_a_command(str)) 
		return ADD;
	if (is_c_command(str))
		return COMP;
	if (is_label_definition(str))
		return LBL;
	return UNKNOWN;
}

static bool is_a_command(const char* str) {
	return str[0] == '@';
}

static bool is_label_definition(const char* str) {
	
	bool has_parenthesis_first = str[0] == '(';
	bool has_parenthesis_last = str[strlen(str) - 1] == ')';

	
	return has_parenthesis_first && has_parenthesis_last && strlen(str) > 2;
}

static bool is_c_command(const char* str) {
	int assignment_operator_position = 0;
	int computation_operation_position = 0;
	int jump_operation_position = 0;
	int computation_termination_position = 0;

	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == '=') {
			assignment_operator_position = i;
			computation_operation_position = i+1;
		}
		if (str[i] == ';') {
			jump_operation_position = i+1;
			computation_termination_position = i;
		}
	}

	if (assignment_operator_position != 0) {
		char *destination = malloc(sizeof(char) * (assignment_operator_position + 1));
		strncpy(destination, str, assignment_operator_position);
		destination[assignment_operator_position] = '\0';
		if (!is_valid_destination(destination)) {
			return false;
		}
		free(destination);
	}

	if (computation_termination_position == 0) {
		computation_termination_position = strlen(str);
	}

	int computation_length = computation_termination_position - computation_operation_position;
	char *computation = malloc(sizeof(char) * (computation_length + 1));
	strncpy(computation, str+computation_operation_position, computation_length);
	computation[computation_length] = '\0';
	
	if (!is_valid_computation(computation)) {
		return false;
	}

	free(computation);

	if (jump_operation_position != 0) {
		int jump_operation_length = strlen(str) - jump_operation_position;
		char *jump_operation = malloc(sizeof(char) * (jump_operation_length + 1));
		strncpy(jump_operation, str+jump_operation_position, jump_operation_length);
		jump_operation[jump_operation_length] = '\0';
		if (!is_valid_jump(jump_operation)) {
			return false;
		}
		free(jump_operation);
	}

	return true;

}

static bool is_valid_destination(const char* str) {
	return is_valid(str, valid_destinations, VALID_DESTINATION_COUNT);
}

static bool is_valid_computation(const char* str) {
	return is_valid(str, valid_computations, VALID_COMPUTATION_COUNT);
}

static bool is_valid_jump(const char* str) {
	return is_valid(str, valid_jumps, VALID_JUMP_COUNT);
}
static bool is_valid(const char* str, const char** argv, unsigned int count) {
	for (int i = 0; i < count; i++) {
		if (strcmp(str, argv[i]) == 0) {
			return true;
		}
	}
	return false;
}


