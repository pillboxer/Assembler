#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../error/Error.h"
#include "HashMap.h"
void strip_spaces (char* dst, const char* src) {

	bool have_reached_printable_char = false;
	int count = 0;
	int length = strlen(src);

 	while(*src != '\0') {
		if (count == length - 1 && *src == '\n')
			break;
		have_reached_printable_char = have_reached_printable_char ? true : isprint(*src);
    	if(have_reached_printable_char && (*src == '\n' || !isspace(*src))) {
			*dst = *src; // then copy
      		dst++;
		}
		count++;
		src++;
	}

  	*dst = '\0';
}

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

void strip_labels(char* dst, const char* src, HashMap* hash_map) {
	
	int current_command = 0;
	bool save_command = false;
	bool new_command = true;
	bool copy = true;
	char current_label[256];
	int current_label_index = 0;
	char last_copied;

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
			save_command = false;
			current_label[current_label_index] = '\0';
			current_command--;
			for (int i = 0; i <= strlen(current_label); i++) {
				char lowered = tolower(current_label[i]);
				current_label[i] = lowered;
			}
			hash_map_put(hash_map, current_label, current_command+1);
			current_label_index = 0;
		}
		if (save_command) {
			current_label[current_label_index++] = *src;
		}
		if (new_command && *src == '(') {
			save_command = true;
			copy = false;
		}		
		if (copy) {
			*dst = *src;
			dst++;
			last_copied = *src;
		}
		src++;
	}
		*dst = '\0';
}

void save_variables(char* dst, HashMap* hash_map) {
	bool is_a_variable_declaration = false;
	char current_variable[256];
	int current_variable_index = 0;

	while(*dst != '\0') {
		if (*dst == '\n') {
			if (is_a_variable_declaration) {
				is_a_variable_declaration = false;
				current_variable[current_variable_index] = '\0';
				char* remaining;
				long int address = strtol(current_variable, &remaining, 10);
				printf("Long int is %ld\n", address);
				if (address == 0) {
				if (hash_map_contains(hash_map, current_variable)) {
						printf("Should ignore saving %s\n", current_variable);
					}
					else {
						printf("Must save variable %s\n", current_variable);
					}
				}
				else {
					printf("It's just a number\n");
				}
				current_variable_index = 0;
			}
		}
		if (is_a_variable_declaration) {
			current_variable[current_variable_index++] = tolower(*dst);
		}
		if (*dst == '@') {
			char next = tolower(*(++dst));
			printf("Next is %c\n", next);
			if (next != 'r' && next != '0') {
				is_a_variable_declaration = true;
			}
			dst--;
		}
		dst++;
	}
	printf("DST is now null\n");

	// Go through the A Instructions
	// Once we've made it through the command, check if we've already saved it
	// Otherwise build up from 16.
}
