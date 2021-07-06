#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../error/Error.h"

void strip_spaces (char* dst, const char* src)
{
	printf("D is %s\n and S is %s\n", dst, src);

  while(*src != '\0')
  {
    if(!isspace(*src)) // if not string
    {
      *dst = *src; // then copy
      dst++;
    }
    src++;
  }
  *dst = '\0';
}

void strip_comments(char* dst, const char* src) {
	bool copy = true;
	int index = 0;
	for (int i = 0; i < strlen(src); i++) {
		if (src[i] == '/' && src[i+1] == '/')
			copy = false;
		if (copy) {
			dst[index++] = src[i];
		}
		if (src[i] == '\n')
			printf("marking copy as true");
			copy = true;
	}

}
