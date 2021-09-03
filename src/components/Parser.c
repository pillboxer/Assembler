#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "HashMap.h"
#include "Parser.h"
#include "../error/Error.h"

#define VALID_DESTINATION_COUNT 7
#define VALID_COMPUTATION_COUNT 28
#define VALID_JUMP_COUNT 7
#define WORD_LENGTH 16

#define A_START 0

#define C_A_OR_M_START 3
#define C_A_OR_M_BIT_LENGTH 1

#define C_DEFAULTS_START 0
#define C_DEFAULTS_BIT_LENGTH 3
#define C_DEFAULTS_ADDRESS 7

#define C_COMP_START 4
#define C_COMP_BIT_LENGTH 6

#define C_JUMP_START 13
#define C_JUMP_BIT_LENGTH 3

#define C_DEST_BIT_LENGTH 3
#define C_DEST_START 10

// Declarations

static bool is_a_command(const char* str); 

static int jump_address(const char* str);
static int destination_address(const char* str);
static int get_computation_address(const char* cmd);
static int get_jump_address(const char* cmd, int jump_operation_position); 
static int get_destination_address(const char* cmd, int assignment_operation_position);

static void parse_c_command(char* dst, const char* cmd);
static void parse_command(char* dst, const char *cmd, HashMap* hash_map); 
static void parse_a_command(char* dst, const char* cmd, HashMap* hash_map); 
static void to_bin(char* cmd, int address, int number_of_places, int starting_position); 
static void get_positions(const char* cmd, int* assignment, int* computation, int *termination, int* jump); 


// *********** CONSTANTS ************** //

// Allowed destinations in C instruction
const char* valid_destinations[VALID_DESTINATION_COUNT] = { "M", "D", "MD", "A", "AM", "AD", "AMD" };
// Allowed computations in C instruction
const char* valid_computations[VALID_COMPUTATION_COUNT] = { "0", "1", "-1", "D", "A", "!D", "!A", "-D", 
															"-A", "D+1", "A+1", "D-1", "A-1", "D+A", "D-A", 
															"A-D", "D&A", "D|A", "M", "!M", "-M", "M+1", 
															"M-1", "D+M", "D-M", "M-D", "D&M", "D|M" };
// Denary representation of a valid computation. 
// For example the instruction D|M (see above) corresponds to 21 -> (101001)
const int valid_computation_integers[VALID_COMPUTATION_COUNT] = { 42, 63, 58, 12, 48, 13, 49, 15, 
																	51, 31, 55, 14, 50, 2, 19,
																	7, 0, 21, 48, 49, 51, 
																	55, 50, 2, 19, 7, 0, 21 }; 
// Allowed jump commands in C instruction
const char* valid_jumps[VALID_JUMP_COUNT] = { "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP" };


// *********** PARSING ************** //

// Parse a source asm file that has been stripped of whitespace and comments
void parse(char* dst, char* src, HashMap* hash_map) {

	char current_label[256];
	int current_label_position = 0;
	int dest_position = 0;

	for (int i = 0; i <= strlen(src); i++) {
	
		if (src[i] == '\n' || src[i] == '\0') {
			// We've either reached a line break or EOF
			current_label[current_label_position] = '\0';
			char parsed[WORD_LENGTH + 1];
			to_bin(parsed, 0, WORD_LENGTH, 0);
			parsed[WORD_LENGTH] = '\0';
			// Parse the current command
			parse_command(parsed, current_label, hash_map);
			for (int j = 0; j < strlen(parsed); j++) {
				// Add the newly parsed command to the output file
				dst[dest_position++] = parsed[j];	
			}
			// Reset label posiion and add \n or \0 to output
			current_label_position = 0;
			dst[dest_position++] = src[i];
			continue;
		}
		current_label[current_label_position++] = src[i];
	}
	// Done, make sure to end with null terminator
	dst[dest_position] = '\0';
}

static void parse_command(char* dst, const char *cmd, HashMap* hash_map) {

	// Can either be A command ("@command") or C command ("D=D+1;JGE")
	if (is_a_command(cmd)) {
		parse_a_command(dst, cmd, hash_map);
	}
	else {
		parse_c_command(dst, cmd);
	}
}

static void parse_c_command(char* dst, const char* cmd) {

	int assignment_operation_position = 0;
	int computation_operation_position = 0;
	int jump_operation_position = 0;
	int computation_termination_position = 0;

	// Fill in parts of parsed command that are common to all C instructions
	to_bin(dst, C_DEFAULTS_ADDRESS, C_DEFAULTS_BIT_LENGTH, C_DEFAULTS_START);
	get_positions(cmd, &assignment_operation_position, &computation_operation_position, &computation_termination_position, &jump_operation_position);

	// Split the command into destination (if applicable), computation and jump (if applicable)

	if (assignment_operation_position != 0) {
		int address = get_destination_address(cmd, assignment_operation_position);
		if (address == -1) {
			exit_with_error(MALFORMED_DESTINATION);
		}
		else { 
			to_bin(dst, address, C_DEST_BIT_LENGTH, C_DEST_START);
		}
	}

	int computation_string_length = computation_termination_position - computation_operation_position;
	char computation_string[computation_string_length + 1];
	strncpy(computation_string, cmd+computation_operation_position, computation_string_length);
	computation_string[computation_string_length] = '\0';
	int computation_address = get_computation_address(computation_string);

	if (computation_address == -1) {
		exit_with_error(MALFORMED_COMPUTATION);
	}
	else {
		to_bin(dst, computation_address, C_COMP_BIT_LENGTH, C_COMP_START);
		for (int i = computation_operation_position; i < computation_termination_position; i++) {
			if (tolower(cmd[i]) == 'm') {
				to_bin(dst, 1, C_A_OR_M_BIT_LENGTH, C_A_OR_M_START);
			}
		}
	}
	
	if (jump_operation_position != 0) {
		int address = get_jump_address(cmd, jump_operation_position);
		if (address == -1) {
			exit_with_error(MALFORMED_JUMP);
		}
		else {
			to_bin(dst, address, C_JUMP_BIT_LENGTH, C_JUMP_START);
		}
	}
}

static void parse_a_command(char* dst, const char* cmd, HashMap* hash_map) {

	size_t cmd_length = strlen(cmd);
	char lowered[cmd_length];
	int index = 0;

	// Disregard '@' by starting at index 1
	for (size_t i = 1; i <= cmd_length; i++) {
		lowered[index++] = tolower(cmd[i]);
	}
	if (!is_integral_string(lowered)) {
		// It's a user-declared variable
		int value = hash_map_get(hash_map, lowered);
		to_bin(dst, value, WORD_LENGTH, A_START);
	}
	else {
		// It's a direct address (eg '@42')
		char address_string[cmd_length];
		strncpy(address_string, lowered, cmd_length);
		int address = atoi(address_string);
		to_bin(dst, address, WORD_LENGTH, A_START);
	}	
}


// *********** ADDRESSES ************** //

// Retreive generic address from array
static int get_address(const char* str, const char** argv, unsigned int count) {

	for (int i = 0; i < count; i++) {
		if (strcmp(str, argv[i]) == 0) {
			return i + 1;
		}
	}
	return -1;
}

// Retrieve destination address from C instruction
static int get_destination_address(const char* cmd, int assignment_operation_position) {

	char destination[assignment_operation_position + 1];
	strncpy(destination, cmd, assignment_operation_position);
	destination[assignment_operation_position] = '\0';
	return destination_address(destination);
}

static int destination_address(const char* str) {
	return get_address(str, valid_destinations, VALID_DESTINATION_COUNT);
}

// Retrieve computation address from C instruction
static int get_computation_address(const char* cmd) {

	for (int i = 0; i < VALID_COMPUTATION_COUNT; i++) {
		if (strcmp(valid_computations[i], cmd) == 0) {
			return valid_computation_integers[i];
		}
	}
	return -1;
}

// Retrieve jump address from C instruction
static int get_jump_address(const char* cmd, int jump_operation_position) {

	int jump_operation_length = strlen(cmd) - jump_operation_position;
	char jump_operation[jump_operation_length + 1];
	strncpy(jump_operation, cmd+jump_operation_position, jump_operation_length);
	jump_operation[jump_operation_length] = '\0';
	return jump_address(jump_operation);

} 

static int jump_address(const char* str) {
   return get_address(str, valid_jumps, VALID_JUMP_COUNT);
}


// *********** HELPER ************** //

static bool is_a_command(const char* str) {
	return str[0] == '@' && strlen(str) > 1;
}

// Is the command pure integers?
bool is_integral_string(char *str) {

	size_t length = strlen(str);
	for (int i = 0; i < length; i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

// Retrieve positions of assignment, computation, 
// and jump instructions as well as the point of termination of a computation
static void get_positions(const char* cmd, int* assignment, int* computation, int *termination, int* jump) {


	size_t length = strlen(cmd);
	for (int i = 0; i < length; i++) {
			if (cmd[i] == '=') {
				*assignment = i;
				*computation = i+1;
			}
			if (cmd[i] == ';') {
				*jump = i+1;
				*termination = i;
			}
		}
		if (*termination == 0) {
			*termination = length;
		}
}

// Convert an address to binary padding with number of places, 
// starting (0 being the 'end' of the string) at starting_position
static void to_bin(char* cmd, int address, int number_of_places, int starting_position) {

	int quo = address;
	int index = number_of_places + starting_position - 1;

	int rem;
	for (int i = 0; i < number_of_places; i++) {
		rem = quo % 2;
		cmd[index--] = rem == 1 ? '1' : '0';
		quo = quo / 2;
	}
}

