#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "Files.h"
#include "../../libs/CuTest.h"
#include "../../src/components/Stripper.h"

void TestStringWithCommentsAndNewLinesIsStripped(CuTest *tc);

void TestFileWithoutLabelsHasExpectedNumberOfCommands(CuTest *tc);
void TestStringContainingLabelsWithCommentsAndNewLinesIsStripped(CuTest *tc);
void TestFileWithLabelsHasExpectedNumberOfCommands(CuTest *tc);
void TestACommandHasCorrectBinaryRepresentation(CuTest *tc);

	// ## SUITES ##

	CuSuite* StripperGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestStringWithCommentsAndNewLinesIsStripped);
		SUITE_ADD_TEST(suite, TestStringContainingLabelsWithCommentsAndNewLinesIsStripped);
		return suite;
	}

	CuSuite* ParserGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestFileWithoutLabelsHasExpectedNumberOfCommands);
		SUITE_ADD_TEST(suite, TestFileWithLabelsHasExpectedNumberOfCommands);
		SUITE_ADD_TEST(suite, TestACommandHasCorrectBinaryRepresentation);
		return suite;
	}


	// ## STRIPPER ##

	void TestStringWithCommentsAndNewLinesIsStripped(CuTest *tc) {
		char *input = malloc(sizeof(char) * (strlen(ADD_ASM_SRC) + 1));
		char *expected = malloc(sizeof(char) * (strlen(ADD_ASM_STRIPPED) + 1));
		strcpy(input, ADD_ASM_SRC);
		strcpy(expected, ADD_ASM_STRIPPED);
		strip(&input);		
		CuAssertStrEquals(tc, expected, input);
		free(input);
		free(expected);
	}

	void TestStringContainingLabelsWithCommentsAndNewLinesIsStripped(CuTest *tc) {
		char *input = malloc(sizeof(char) * (strlen(MAX_ASM_SRC) + 1));
		char *expected = malloc(sizeof(char) * (strlen(MAX_ASM_STRIPPED) + 1));
		strcpy(input, MAX_ASM_SRC);
		strcpy(expected, MAX_ASM_STRIPPED);
		strip(&input);		
		CuAssertStrEquals(tc, expected, input);
		free(input);
		free(expected);
	}

	// ## PARSER ##

	// # Commands
	void TestFileWithoutLabelsHasExpectedNumberOfCommands(CuTest *tc) {
		int expected = 6;
		int actual = num_commands(ADD_ASM_STRIPPED);
		CuAssertIntEquals(tc, expected, actual);
	}

	void TestFileWithLabelsHasExpectedNumberOfCommands(CuTest *tc) {
		int expected = 16;
		int actual = num_commands(MAX_ASM_STRIPPED);
		CuAssertIntEquals(tc, expected, actual);
	}

	void TestACommandHasCorrectBinaryRepresentation(CuTest *tc) {
		char *expected = "00001";
		const char *actual = parsed_a_command("@3i3");
		CuAssertStrEquals(tc, expected, (char*)actual);
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
