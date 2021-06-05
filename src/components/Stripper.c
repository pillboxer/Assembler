#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void strip_whitespace(char **ptr);

void strip(char** ptr) {
	strip_whitespace(ptr);
}


static void strip_whitespace(char **ptr) {
	
	char *copy = *ptr;
	char *res = malloc(sizeof(*ptr));
	int copy_pos = 0;

	for (int i = 0; i < strlen(*ptr); i++) {
		printf("i is now %i\n", i);
		if (copy[i] == '\n' && copy[i-1] == '\n') {
			continue;
		}
		printf("Copy[i] is %c\n", copy[i]);
		res[copy_pos] = copy[i];
		copy_pos++; 
		printf("Res is now %s\n", res);
	}
	*ptr = res;
}
