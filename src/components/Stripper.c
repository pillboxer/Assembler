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

HashMap* strip_labels(char* dst, const char* src) {

	int current_command = 0;
	bool save_command = false;
	bool new_command = true;
	bool copy = true;
	char current_label[256];
	int current_label_index = 0;
	char last_copied;
	HashMap* hash_map = hash_map_create();
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
		return hash_map;
}
