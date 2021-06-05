#include "../../libs/CuTest.h"
#include "../../src/components/Stripper.h"
#include <stdio.h>

void TestStringWithNewlinesReturnsStringWithoutNewLines(CuTest *tc);

	CuSuite* StripperGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestStringWithNewlinesReturnsStringWithoutNewLines);
		return suite;
	}

	void TestStringWithNewlinesReturnsStringWithoutNewLines(CuTest *tc) {
		char* input = "This is a string\n\nAndSoIsThis";
		char* expected = "This is a string\nAndSoIsThis";
		strip(&input);		
		CuAssertStrEquals(tc, expected, input);
	}

	int main() {
		CuSuite* suite = CuSuiteNew();
		CuSuite* strip_suite = StripperGetSuite();
		CuSuiteAddSuite(suite, strip_suite);

		CuString* output = CuStringNew();

		// Getting seg fault here
		CuSuiteRun(suite);
		CuSuiteSummary(suite, output);
		CuSuiteDetails(suite, output);
		printf("%s\n", output->buffer);
	}
