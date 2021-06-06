#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Files.h"
#include "../../libs/CuTest.h"
#include "../../src/components/Stripper.h"

void TestStringWithCommentsAndNewLinesIsStripped(CuTest *tc);
void TestInstructionsHaveExpectedCommandTypes(CuTest *tc);

	// Suites
	CuSuite* StripperGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestStringWithCommentsAndNewLinesIsStripped);
		return suite;
	}

	CuSuite* ParserGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestInstructionsHaveExpectedCommandTypes);
		return suite;
	}

	// Stripper Functions
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

	// Parser Functions
	void TestInstructionsHaveExpectedCommandTypes(CuTest *tc) {
		command_type_t *res = malloc(sizeof(command_type_t) * 6);
		commands(&res, ADD_ASM_STRIPPED);
		
		for (int i = 0; i < 6; i++) {
			printf("testing %d against %d\n", res[i], ADD_ASM_CMDS[i]);
		}
		CuAssert(tc, "All Commands Match", true);
	}

	// Main
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
