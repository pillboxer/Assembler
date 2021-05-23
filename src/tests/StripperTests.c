#include "../../libs/CuTest.h"

	void TestStringWithCommentsReturnsStringWithoutComments(CuTest *tc);

	CuSuite* StripperGetSuite() {
		CuSuite* suite = CuSuiteNew();
		SUITE_ADD_TEST(suite, TestStringWithCommentsReturnsStringWithoutComments);
		return suite;
	}

	void TestStringWithCommentsReturnsStringWithoutComments(CuTest *tc) {
		char* input = "Some input";
		char* some_output = "Some output";
		char* expected = "Some output";
		CuAssertStrEquals(tc, expected, input);
	}
