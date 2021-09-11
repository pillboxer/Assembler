#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "Files.h"
#include "../../libs/CuTest.h"
#include "../components/Stripper.h"
#include "../components/Parser.h"

void TestProgramIsParsedCorrectly(CuTest *tc);

CuSuite* ParserGetSuite() {
CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestProgramIsParsedCorrectly);
	return suite;
}


void TestProgramIsParsedCorrectly(CuTest *tc) {
	
	const char* file_source;
	const char* file_stripped;
	const char* file_parsed;
	char file_name[5];

	#ifdef MAX	// make tests_max
		strcpy(file_name, "MAX");
		file_source = MAX_ASM_SRC;
		file_stripped = MAX_ASM_STRIPPED;
		file_parsed = MAX_ASM_PARSED;
	#elif RECT	// make tests_rect
		strcpy(file_name, "RECT");
		file_source = RECT_ASM_SRC;
		file_stripped = RECT_ASM_STRIPPED;
		file_parsed = RECT_ASM_PARSED;
	#elif PONG	// make tests_pong
		strcpy(file_name, "PONG");
		file_source = PONG_ASM_SRC;
		file_stripped = PONG_ASM_STRIPPED;
		file_parsed = PONG_ASM_PARSED;
	#else	// make tests
		strcpy(file_name, "ADD");	
		file_source = ADD_ASM_SRC;
		file_stripped = ADD_ASM_STRIPPED;
		file_parsed = ADD_ASM_PARSED;
	#endif
	
	printf("Testing file %s\n", file_name);

	char no_comments[strlen(file_source) + 1];
	char parsed[strlen(file_parsed) + 1];
	char no_whitespace[strlen(file_stripped) + 1];
	char no_labels[strlen(file_stripped + 10000)];
	char src[strlen(file_source) + 1];

	strcpy(src, file_source);
	strip_comments(no_comments, src);
	strip_spaces(no_whitespace, no_comments);
	HashMap* hash_map = hash_map_create();
	strip_labels(no_labels, no_whitespace, hash_map);
	save_variables(no_labels, hash_map);
	parse(parsed, no_labels, hash_map);
	CuAssertStrEquals(tc, (char*)file_parsed, parsed);
}

int main() {

	CuSuite* suite = CuSuiteNew();

	CuSuite* parse_suite = ParserGetSuite();
	CuSuiteAddSuite(suite, parse_suite);

	CuString* output = CuStringNew();

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);

	printf("%s\n", output->buffer);
}
