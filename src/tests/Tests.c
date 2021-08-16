#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "Files.h"
#include "../../libs/CuTest.h"
#include "../../src/components/Stripper.h"

void TestStringWithCommentsAndNewLinesIsStripped(CuTest *tc);

void TestProgramIsParsedCorrectly(CuTest *tc);
void TestStringContainingLabelsWithCommentsAndNewLinesIsStripped(CuTest *tc);

	CuSuite* ParserGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestProgramIsParsedCorrectly);
		return suite;
	}


	// ## PARSER ##
	void TestProgramIsParsedCorrectly(CuTest *tc) {
		
		
		const char* file_source;
		const char* file_stripped;
		const char* file_parsed;

		#ifdef MAX
			file_source = MAX_ASM_SRC;
			file_stripped = MAX_ASM_STRIPPED;
			file_parsed = MAX_ASM_PARSED;
		#else
			file_source = ADD_ASM_SRC;
			file_stripped = ADD_ASM_STRIPPED;
			file_parsed = ADD_ASM_PARSED;
		#endif

		printf("source is %s\n", file_source);			
		char no_comments[strlen(file_source) + 1];
		char parsed[strlen(file_parsed) + 1];
		char no_whitespace[strlen(file_stripped) + 1];
		char no_labels[strlen(file_stripped + 1)];
		char src[strlen(file_source) + 1];
		strcpy(src, file_source);
		strip_comments(no_comments, src);
		strip_spaces(no_whitespace, no_comments);
		HashMap* hash_map = strip_labels(no_labels, no_whitespace);
		parse(parsed, no_labels, hash_map);
		CuAssertStrEquals(tc, (char*)file_parsed, parsed);
	}

	// ## MAIN ##
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
