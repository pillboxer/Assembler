#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Files.h"
#include "../../libs/CuTest.h"
#include "../../src/components/Stripper.h"

void TestStringWithCommentsAndNewLinesIsStripped(CuTest *tc);

void TestFileHasExpectedNumberOfCommands(CuTest *tc);
void TestFileHasCorrectTypeOfCommands(CuTest *tc);
void TestInvalidDestinationIsNotValid(CuTest *tc);
void TestInvalidDestinationIsNotValid(CuTest *tc);
void TestValidDestinationIsValid(CuTest *tc);

	// ## SUITES ##

	CuSuite* StripperGetSuite() {
		CuSuite* suite = CuSuiteNew();
//		SUITE_ADD_TEST(suite, TestStringWithCommentsAndNewLinesIsStripped);
		return suite;
	}

	CuSuite* ParserGetSuite() {
		CuSuite* suite = CuSuiteNew();
//		SUITE_ADD_TEST(suite, TestFileHasExpectedNumberOfCommands);
//		SUITE_ADD_TEST(suite, TestFileHasCorrectTypeOfCommands);
		SUITE_ADD_TEST(suite, TestInvalidDestinationIsNotValid);
		SUITE_ADD_TEST(suite, TestValidDestinationIsValid);
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


	// ## PARSER ##

	// # Commands
	void TestFileHasExpectedNumberOfCommands(CuTest *tc) {
		int expected = 6;
		int actual = num_commands(ADD_ASM_STRIPPED);
		CuAssertIntEquals(tc, expected, actual);
	}
	
	void TestFileCorrectTypeOfCommands(CuTest *tc) {
		command_type_t expected = COMP;
		command_type_t actual = command_type("AMD=FIX");
		CuAssertIntEquals(tc, actual, expected);
	}

	// # Destinations
	void TestInvalidDestinationIsNotValid(CuTest *tc) {
		bool expected = false;
		bool actual = is_valid_destination("AMM");
		CuAssertIntEquals(tc, actual, expected);
	}

	void TestValidDestinationIsValid(CuTest *tc) {
		bool expected = true;
		bool actual = is_valid_destination("AM");
		CuAssertIntEquals(tc, actual, expected);
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
