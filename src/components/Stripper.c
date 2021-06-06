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
	
	char *copy = *ptr;
	char *res = malloc(sizeof(*ptr));
	int copy_pos = 0;

	for (int i = 0; i < strlen(*ptr); i++) {
		if (copy[i] == '\n' && copy[i-1] == '\n') {
			continue;
		}
		res[copy_pos] = copy[i];
		copy_pos++; 
	}
	res[copy_pos] = '\0';
	*ptr = res;
}

static void strip_comments(char **ptr) {

	char *copy = *ptr;
	char *res = malloc(sizeof(*ptr));
	int copy_pos = 0;
	bool copy_allowed = true; 	

	for (int i = 0; i < strlen(*ptr); i++) {

		if (copy[i] == '\n' && !copy_allowed) { 
			copy_allowed = true;
			continue;
		}

		if (copy[i] == '/' && copy[i+1] == '/') {
			copy_allowed = false;
			continue;
		}

		if (copy_allowed) {
			res[copy_pos] = copy[i];
			copy_pos++;
		}
	}
	
	res[copy_pos] = '\0';
	*ptr = res;
}
