#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../error/Error.h"

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
	int index = 0;
	for (int i = 0; i < strlen(src); i++) {
		if (src[i] == '\n')
			copy = true;
		if (src[i] == '/' && src[i+1] == '/')
			copy = false;
		if (copy) {
			dst[index++] = src[i];
		}
	
	}
	dst[index] = '\0';
	printf("After stripping comments %s\n", dst);
}
