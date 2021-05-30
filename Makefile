all_objects = $(src_objects)
test_objects = StripperTests.o CuTest.o 
src_objects = main.o Error.o

tests: 
	gcc libs/CuTest.c src/components/*.c src/tests/*.c -o Tests
	


clean:
	rm -f *.o Tests
