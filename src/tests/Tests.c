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
void TestLabelACommandHasCorrectBinaryRepresentation(CuTest *tc);
void TestPurelyNumericACommandHasCorrectBinaryRepresentation(CuTest *tc);
void TestRegisterACommandHasCorrectBinaryRepresentation(CuTest *tc);

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
		SUITE_ADD_TEST(suite, TestLabelACommandHasCorrectBinaryRepresentation);
		SUITE_ADD_TEST(suite, TestPurelyNumericACommandHasCorrectBinaryRepresentation);
		SUITE_ADD_TEST(suite, TestRegisterACommandHasCorrectBinaryRepresentation);

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

	void TestLabelACommandHasCorrectBinaryRepresentation(CuTest *tc) {
		char *expected = "00001";
		const char *actual = parsed_a_command("@3i3");
		CuAssertStrEquals(tc, expected, (char*)actual);
	}

	void TestRegisterACommandHasCorrectBinaryRepresentation(CuTest *tc) {
		char *expected = "0000000000001101";
		const char *actual = parsed_a_command("@R13");
		CuAssertStrEquals(tc, expected, (char*)actual);
	}
	void TestPurelyNumericACommandHasCorrectBinaryRepresentation(CuTest *tc) {
		char *expected = "0000100101010111";
		const char* actual = parsed_a_command("@2391");
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
