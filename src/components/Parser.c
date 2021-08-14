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
#define DESTINATION_BIT_LENGTH 3
#define COMPUTATION_BIT_LENGTH 6

// ********************************* //

// Constants
const char* valid_destinations[VALID_DESTINATION_COUNT] = { "M", "D", "MD" "A", "AM", "AD", "AMD" };
const char* valid_computations[VALID_COMPUTATION_COUNT] = { "0", "1", "-1", "D", "A", "!D", "!A", "-D", 
															"-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", 
															"A-D", "D&A", "D|A", "M", "!M", "-M", "M+1", 
															"M-1", "D+M", "D-M", "M-D", "D&M", "D|M" };
const int valid_computation_integers[VALID_COMPUTATION_COUNT] = { 42, 63, 58, 12, 48, 13, 49, 15, 
																	51, 31, 55, 14, 50, 2, 19,
																	7, 0, 21, 48, 49, 51, 
																	55, 50, 2, 19, 7, 0, 21 }; 
const char* valid_jumps[VALID_JUMP_COUNT] = { "JGT", "JGE", "JEQ", "JLT", "JLE", "JMP", "JNE" };

// A Commands
static bool is_a_command(const char* str);

// C Commands
// remove this, just parse. Exit within parse if something goes wrong
static bool is_c_command(const char* str);

static int get_destination_address(const char* str, int assigment_operator_position);
static int destination_address(const char* str);

static int get_computation_address(const char* str);

static int jump_address(const char* str);
static int get_address(const char* str, const char** argv, unsigned int count);

// Parsed
static const char* parsed_command(char *cmd, HashMap* hash_map);
static const char* parsed_a_command(const char* cmd, HashMap* hash_map);
static const char* parsed_c_command(const char* cmd);
static const char* to_bin(int address, int number_of_places);

// ********************************* //



void parse(char* dst, char* src, HashMap* hash_map) {
	char current_label[256];
	int current_label_position = 0;
	int dest_position = 0;
	for (int i = 0; i <= strlen(src); i++) {
		if (src[i] == '\n' || src[i] == '\0') {
			current_label[current_label_position] = '\0';
			const char* parsed = parsed_command(current_label, hash_map);
			for (int j = 0; j < strlen(parsed); j++) {
				dst[dest_position++] = parsed[j];	
			}
			current_label_position = 0;
			dst[dest_position++] = '\n';
			continue;
		}
			current_label[current_label_position++] = src[i];
	}
	dst[dest_position] = '\0';
}

static const char* parsed_command(char *cmd, HashMap* hash_map) {
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

const char* parsed_a_command(const char* cmd, HashMap* hash_map) {
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
		return to_bin(value, WORD_LENGTH);
	}
	else {
		char address_string[cmd_length];
		strncpy(address_string, lowered, cmd_length);
		int address = atoi(address_string);
		return to_bin(address, WORD_LENGTH);
	}	
}



static bool is_a_command(const char* str) {
	return str[0] == '@' && strlen(str) > 1;
}

static int get_positions(const char* cmd, int* assignment, int* computation, int *termination) {
	size_t length = strlen(cmd);
	for (int i = 0; i < length; i++) {
			if (cmd[i] == '=') {
				*assignment = i;
				*computation = i+1;
			}
			if (cmd[i] == ';') {
				//jump_operation_position = i+1;
				*termination = i;
			}
		}
		if (*termination == 0) {
			*termination = length;
		}
}

static int get_destination_address(const char* cmd, int assignment_operation_position) {
	char destination[assignment_operation_position + 1];
	strncpy(destination, cmd, assignment_operation_position);
	printf("now checking destination %s\n", destination);
	return destination_address(destination);
}

static int get_computation_address(const char* cmd) {

	for (int i = 0; i < VALID_COMPUTATION_COUNT; i++) {
		if (strcmp(valid_computations[i], cmd) == 0) {
			return valid_computation_integers[i];
		}
	}

	return -1;

}

static bool is_c_command(const char* cmd) {

	int assignment_operation_position = 0;
	int computation_operation_position = 0;
	int jump_operation_position = 0;
	int computation_termination_position = 0;
	get_positions(cmd, &assignment_operation_position, &computation_operation_position, &computation_termination_position);

	if (assignment_operation_position != 0) {
		int address = get_destination_address(cmd, assignment_operation_position);
		if (address == -1) {
			exit_with_error(MALFORMED_DESTINATION);
		}
		else { 
			printf("Destination address for cmd %s is %s\n", cmd, to_bin(address, DESTINATION_BIT_LENGTH));
		}
	}

	int computation_string_length = computation_termination_position - computation_operation_position;
	char computation_string[computation_string_length + 1];
	strncpy(computation_string, cmd+computation_operation_position, computation_string_length);
	computation_string[computation_string_length] = '\0';
	int computation_address = get_computation_address(computation_string);

	if (computation_address == -1) {
		exit_with_error(MISSING_COMPUTATION);
	}
	else {
		printf("Your computation for %s is %s\n", computation_string, to_bin(computation_address, COMPUTATION_BIT_LENGTH));
	}

//
//	free(computation);
//
//	if (jump_operation_position != 0) {
//		int jump_operation_length = strlen(cmd) - jump_operation_position;
//		char *jump_operation = malloc(sizeof(char) * (jump_operation_length + 1));
//		if (jump_operation == NULL)
//			exit_with_error(NULL_POINTER);
//		strncpy(jump_operation, cmd+jump_operation_position, jump_operation_length);
//		jump_operation[jump_operation_length] = '\0';
//		if (is_valid_jump(jump_operation)) {
//			return false;
//		}
//		free(jump_operation);
//	}
	return true;
}


static int destination_address(const char* str) {
	return get_address(str, valid_destinations, VALID_DESTINATION_COUNT);
}

static int jump_address(const char* str) {
	return get_address(str, valid_jumps, VALID_JUMP_COUNT);
}
static int get_address(const char* str, const char** argv, unsigned int count) {
	for (int i = 0; i < count; i++) {
		if (strcmp(str, argv[i]) == 0) {
			return i + 1;
		}
	}
	return -1;
}

static const char* to_bin(int address, int number_of_places) {
	char *binary_address = malloc(number_of_places + 1);

	if (binary_address == NULL)
		exit_with_error(NULL_POINTER);

	for (int i = 0; i <= number_of_places; i++) {
		if (i == number_of_places) {
			binary_address[i] = '\0';
		}
		else {
			binary_address[i] = '0';
		}
	}

	int quo = address;
	int rem;

	for (int i = 1; i <= number_of_places; i++) {
		if (quo == 0)
			break;
		rem = quo % 2;
		binary_address[number_of_places - i] = rem == 1 ? '1' : '0';
		quo = quo / 2;
	}
	return binary_address;
}
