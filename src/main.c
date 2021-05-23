#include "../libs/CuTest.h"
#include "tests/StripperTests.h"
#include "components/Stripper.h"
#include "error/Error.h"
#include <stdio.h>

void run_all_tests() {
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();
	CuSuiteAddSuite(suite, StripperGetSuite());
	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	fflush(stdout);
	// printf("%s\n", output->buffer);
}

int main(int argc, char** argv) {
	
	switch (argc) 
	{
		case 2:
			printf("Test: %s", argv[1]);
			break;
		default:
			exit_with_error(INCORRECT_ARGUMENT_COUNT);		
	}
		
	run_all_tests();
	return 0;
}
