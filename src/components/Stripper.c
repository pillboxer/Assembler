#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../error/Error.h"

static void strip_whitespace(char **ptr);
static void strip_comments(char **ptr);

void strip(char** ptr) {
	strip_whitespace(ptr);
	strip_comments(ptr);
}

static void strip_whitespace(char **ptr) {
	
	char *res = malloc(strlen(*ptr) + 1);
	if (res == NULL)
		exit_with_error(NULL_POINTER);
	int current_pos = 0;

	for (int i = 0; i < strlen(*ptr); i++) {
		if (((*ptr)[i] == ' ') || ((*ptr)[i] == '\n' && ((*ptr)[i-1] == '\n' || (*ptr)[i+1] == '\0'))) {
			continue;
		}
		res[current_pos] = (*ptr)[i];
		current_pos++; 
	}
	res[current_pos] = '\0';
	free(*ptr);
	*ptr = res;
}

static void strip_comments(char **ptr) {
	char *res = malloc(strlen(*ptr) + 1);
	if (res == NULL)
		exit_with_error(NULL_POINTER);
	int current_pos = 0;
	bool copy_allowed = true; 	
	bool is_purely_comment = false;

	for (int i = 0; i < strlen(*ptr); i++) {

		if ((*ptr)[i] == '\n' && !copy_allowed) { 
			copy_allowed = true;
			if (is_purely_comment)
				continue;
		}

		if ((*ptr)[i] == '/' && (*ptr)[i+1] == '/') {
			is_purely_comment = (*ptr)[i-1] == '\n' || i == 0;
			copy_allowed = false;
			continue;
		}

		if (copy_allowed) {
			res[current_pos] = (*ptr)[i];
			current_pos++;
		}
	}
	
	res[current_pos] = '\0';
	free(*ptr);
	*ptr = res;
}
