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
	// ## SUITES ##

	CuSuite* StripperGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestStringWithCommentsAndNewLinesIsStripped);
		SUITE_ADD_TEST(suite, TestStringContainingLabelsWithCommentsAndNewLinesIsStripped);
		return suite;
	}

	CuSuite* ParserGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestProgramIsParsedCorrectly);
		return suite;
	}


	// ## STRIPPER ##

	void TestStringWithCommentsAndNewLinesIsStripped(CuTest *tc) {
		char no_comments[strlen(ADD_ASM_SRC) + 1];
		char src[strlen(ADD_ASM_SRC) + 1];
		char no_whitespace[strlen(ADD_ASM_SRC) + 1];
		strcpy(src, ADD_ASM_SRC);
		strip_comments(no_comments, src);
		strip_spaces(no_whitespace, no_comments);
		CuAssertStrEquals(tc, (char*)ADD_ASM_STRIPPED, no_whitespace);
	}

	void TestStringContainingLabelsWithCommentsAndNewLinesIsStripped(CuTest *tc) {
		char no_comments[strlen(MAX_ASM_SRC) + 1];
		char no_whitespace[strlen(MAX_ASM_STRIPPED) + 1];
		char no_labels[strlen(MAX_ASM_STRIPPED + 1)];
		char src[strlen(MAX_ASM_SRC) + 1];
		strcpy(src, MAX_ASM_SRC);
		strip_comments(no_comments, src);
		strip_spaces(no_whitespace, no_comments);
		strip_labels(no_labels, no_whitespace);
		CuAssertStrEquals(tc, (char*)MAX_ASM_STRIPPED, no_labels);
	}

	// ## PARSER ##
	void TestProgramIsParsedCorrectly(CuTest *tc) {
		char no_comments[strlen(MAX_ASM_SRC) + 1];
		char parsed[strlen(MAX_ASM_PARSED) + 1];
		char no_whitespace[strlen(MAX_ASM_STRIPPED) + 1];
		char no_labels[strlen(MAX_ASM_STRIPPED + 1)];
		char src[strlen(MAX_ASM_SRC) + 1];
		strcpy(src, MAX_ASM_SRC);
		strip_comments(no_comments, src);
		strip_spaces(no_whitespace, no_comments);
		HashMap* hash_map = strip_labels(no_labels, no_whitespace);
		parse(parsed, no_labels, hash_map);
		CuAssertStrEquals(tc, (char*)MAX_ASM_PARSED, parsed);
	}

	// ## MAIN ##
	int main() {
		CuSuite* suite = CuSuiteNew();

		CuSuite* strip_suite = StripperGetSuite();
		CuSuiteAddSuite(suite, strip_suite);

		CuSuite* parse_suite = ParserGetSuite();
		CuSuiteAddSuite(suite, parse_suite);

		CuString* output = CuStringNew();

		CuSuiteRun(suite);
		CuSuiteSummary(suite, output);
		CuSuiteDetails(suite, output);

		printf("%s\n", output->buffer);
	}
