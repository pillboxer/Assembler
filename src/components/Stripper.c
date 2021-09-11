#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "HashMap.h"
#include "Parser.h"

const int SCREEN_ADDRESS = 16384;
const int KBD_ADDRESS = 24576;
const int SP_ADDRESS = 0;
const int LCL_ADDRESS = 1;
const int ARG_ADDRESS = 2;
const int THIS_ADDRESS = 3;
const int THAT_ADDRESS = 4;

// Removes extraneous white space and blank lines
void strip_spaces (char* dst, const char* src) {

	bool have_reached_printable_char = false;
	const int length = strlen(src);

	for (int count = 0; *src != '\0'; ++count, ++src) {
		if (count == length - 1 && *src == '\n') {
			break;
		}
		have_reached_printable_char |= isprint(*src); 
    	if(have_reached_printable_char && (*src == '\n' || !isspace(*src))) {
			*dst = *src; // then copy
      		dst++;
		}
	}
  	*dst = '\0';
}

// Remove comments (like this!) from file
void strip_comments(char* dst, const char* src) {

	bool copy = true;

	for (int i = 0; i < strlen(src); i++) {
		if (src[i] == '\n')
			copy = true;
		if (src[i] == '/' && src[i+1] == '/')
			copy = false;
		if (copy) {
			*dst = src[i];
			dst++;
		}
	}
	*dst = '\0';
}

// Map particular variables to corresponding address
static void map_reserved_variables(HashMap* hash_map) {
	
	hash_map_put(hash_map, "screen", SCREEN_ADDRESS);
	hash_map_put(hash_map, "kbd", KBD_ADDRESS);
	hash_map_put(hash_map, "sp", SP_ADDRESS);
	hash_map_put(hash_map, "lcl", LCL_ADDRESS);
	hash_map_put(hash_map, "arg", ARG_ADDRESS);
	hash_map_put(hash_map, "this", THIS_ADDRESS);
	hash_map_put(hash_map, "that", THAT_ADDRESS);

	char reg_name[5];
	for (int i = 0; i < 16; ++i) {
		snprintf(reg_name, 5, "r%d", i);
		hash_map_put(hash_map, reg_name, i);
	}
}

	

// Remove label definitions and replace them with corresponding line number of
// logic following the definition
void strip_labels(char* dst, const char* src, HashMap* hash_map) {

	map_reserved_variables(hash_map);

	int current_command = 0;
	bool save_command = false;
	bool new_command = true;
	bool copy = true;
	char current_label[256];
	int current_label_index = 0;
	char last_copied;

	printf("DST is %s\n", dst);

	while(*src != '\0') {
		if (*src == '\n') {
			new_command = true;
			if (last_copied == '\n') {
				src++;
			}
			current_command++;
			copy = true;
		}

		if (*src == ')' && save_command) {	
			printf("Blah");
			save_command = false;
			current_label[current_label_index] = '\0';
			// Move backwards to go back to the command we were dealing with
			current_command--;
			for (int i = 0; i <= strlen(current_label); i++) {
				char lowered = tolower(current_label[i]);
				printf("Index is %d\n", i);
				current_label[i] = lowered;
			}
			// Now move forward one line and save whatever command number that is
			hash_map_put(hash_map, current_label, current_command+1);
			current_label_index = 0;
		}
		if (save_command) {
			printf("Accessing index %d\n", current_label_index + 1);
			current_label[current_label_index++] = *src;
		}
		if (new_command && *src == '(') {
			save_command = true;
			copy = false;
		}		
		if (copy) {
			printf("Copyying %c\n", *src);
			*dst = *src;
			
			dst++;
			last_copied = *src;
		}
		src++;
	}
		*dst = '\0';
}

// Save any user declared variables
void save_variables(char* dst, HashMap* hash_map) {
	bool is_a_variable_declaration = false;
	char current_variable[256];
	int current_variable_index = 0;
	int current_variable_address = 16;

	while(*dst != '\0') {

		if (*dst == '\n') {
			if (is_a_variable_declaration) {
				is_a_variable_declaration = false;
				current_variable[current_variable_index] = '\0';
				current_variable_index = 0;

				if (!is_integral_string(current_variable)) {
					if (hash_map_contains(hash_map, current_variable)) {
						// It's a label declaration that we've already saved in the method above
						continue;
					}
					else {
						hash_map_put(hash_map, current_variable, current_variable_address++);
					}
				}
			}
		}
		if (is_a_variable_declaration) {
			current_variable[current_variable_index++] = tolower(*dst);
		}
		if (*dst == '@') {
			char next = tolower(*(++dst));
			if (next != 'r' && next != '0') {
				is_a_variable_declaration = true;
			}
			dst--;
		}
		dst++;
	}
}
