#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static void strip_whitespace(char **ptr);
static void strip_comments(char **ptr);

void strip(char** ptr) {
	strip_whitespace(ptr);
	strip_comments(ptr);
}


static void strip_whitespace(char **ptr) {
	
	char *res = malloc(sizeof(*ptr));
	int current_pos = 0;

	for (int i = 0; i < strlen(*ptr); i++) {
		if ((*ptr)[i] == '\n' && ((*ptr)[i-1] == '\n' || (*ptr)[i+1] == '\0')) {
			continue;
		}
		res[current_pos] = (*ptr)[i];
		current_pos++; 
	}
	res[current_pos] = '\0';
	*ptr = res;
}

static void strip_comments(char **ptr) {

	char *res = malloc(sizeof(*ptr));
	int current_pos = 0;
	bool copy_allowed = true; 	

	for (int i = 0; i < strlen(*ptr); i++) {

		if ((*ptr)[i] == '\n' && !copy_allowed) { 
			copy_allowed = true;
			continue;
		}

		if ((*ptr)[i] == '/' && (*ptr)[i+1] == '/') {
			copy_allowed = false;
			continue;
		}

		if (copy_allowed) {
			res[current_pos] = (*ptr)[i];
			current_pos++;
		}
	}
	
	res[current_pos] = '\0';
	*ptr = res;
}
