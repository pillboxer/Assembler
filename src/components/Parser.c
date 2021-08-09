#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include <ctype.h>
#include "../error/Error.h"

#define VALID_DESTINATION_COUNT 7
#define VALID_COMPUTATION_COUNT 28
#define VALID_JUMP_COUNT 7
#define WORD_LENGTH 16

static command_type_t command_type(const char* str);

// Constants
const char* valid_destinations[VALID_DESTINATION_COUNT] = { "D", "M", "A", "MD", "AM", "AD", "AMD" };
const char* valid_computations[VALID_COMPUTATION_COUNT] = { "0", "1", "-1", "D", "A", "!D", "!A", "-D", 
															"-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", 
															"A-D", "D&A", "D|A", "M", "!M", "-M", "M+1", 
															"M-1", "D+M", "D-M", "M-D", "D&M", "D|M" };
const char* valid_jumps[VALID_JUMP_COUNT] = { "JGT", "JGE", "JEQ", "JLT", "JLE", "JMP", "JNE" };



// A Commands
static bool is_a_command(const char* str);
static const char* to_bin(int address);

// C Commands
static bool is_c_command(const char* str);
static bool is_valid_destination(const char* str);
static bool is_valid_computation(const char* str);
static bool is_valid_jump(const char* str);
static bool is_valid(const char* str, const char** argv, unsigned int count);

// Should ultimately be static
const char* parsed_a_command(const char* cmd, HashMap* hash_map);

static const char* parse_command(char *cmd, HashMap* hash_map) {
	if (is_a_command(cmd)) {
		return parsed_a_command(cmd, hash_map);
	}
	else if (is_c_command(cmd)) {
		return "Parse C";
	}
	else {
		exit_with_error(UNKNOWN_COMMAND);
	}
}

void parse(char* dst, char* src, HashMap* hash_map) {
	char current_label[256];
	int current_label_position = 0;
	for (int i = 0; i < strlen(src); i++) {
		if (src[i] == '\n') {
			current_label[current_label_position] = '\0';
			parse_command(current_label, hash_map);
			current_label_position = 0;
			continue;
		}
			current_label[current_label_position++] = src[i];
	}
}


const char* parsed_a_command(const char* cmd, HashMap* hash_map) {
	char *parsed = calloc(WORD_LENGTH + 1, sizeof(char));
	size_t cmd_length = strlen(cmd);
	char lowered[cmd_length];

	// No need for size to consider null terminator since we disregard '@'
	memset(lowered, 0, cmd_length*sizeof(char));
	int index = 0;

	// Disregard '@' and 'R'
	for (size_t i = 1; i < cmd_length; i++) {
		if (i == 0 || (i == 1 && tolower(cmd[i]) == 'r'))
			continue;
		lowered[index++] = tolower(cmd[i]);
	}

	char *remaining;
	strtol(lowered, &remaining, 10);

	if (strlen(remaining) != 0) {
		int value = hash_map_get(hash_map, remaining);
		printf("Value of %s is %d\n", remaining, value);
	}

	else {
		char address_string[cmd_length];
		strncpy(address_string, lowered, cmd_length);
		int address = atoi(address_string);
		return to_bin(address);
	}	
	return parsed;
}

const char* to_bin(int address) {
	char *binary_address = malloc(WORD_LENGTH + 1);

	if (binary_address == NULL)
		exit_with_error(NULL_POINTER);

	binary_address = strcpy(binary_address, "0000000000000000");
	int quo = address;
	int rem;

	for (int i = 1; i <= WORD_LENGTH; i++) {
		if (quo == 0)
			break;
		rem = quo % 2;
		binary_address[WORD_LENGTH - i] = rem == 1 ? '1' : '0';
		quo = quo / 2;
	}
	return binary_address;
}

int num_commands(const char* str) {	
	int count = 0;
	int current_size = 1;
	int current_command_pos = 0;
	char *current_command = malloc(sizeof(char) * current_size);

	if (current_command == NULL)
		exit_with_error(NULL_POINTER);
	
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
	return UNKNOWN;
}

static bool is_a_command(const char* str) {
	return str[0] == '@' && strlen(str) > 1;
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
	if (computation == NULL)
		exit_with_error(NULL_POINTER);
	strncpy(computation, str+computation_operation_position, computation_length);
	computation[computation_length] = '\0';
	
	if (!is_valid_computation(computation)) {
		return false;
	}

	free(computation);

	if (jump_operation_position != 0) {
		int jump_operation_length = strlen(str) - jump_operation_position;
		char *jump_operation = malloc(sizeof(char) * (jump_operation_length + 1));
		if (jump_operation == NULL)
			exit_with_error(NULL_POINTER);
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


