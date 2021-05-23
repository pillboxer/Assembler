all_objects = $(src_objects) $(test_objects)
test_objects = StripperTests.o CuTest.o 
src_objects = main.o Error.o 

assembler: main 
	gcc -o assembler $(all_objects)

main: tests
	gcc -c src/main.c

tests: error
	gcc -c src/tests/StripperTests.c
	gcc -c libs/CuTest.c

error:
	gcc -c src/error/Error.c

clean:
	rm -f *.o
