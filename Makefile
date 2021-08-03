tests_dir = src/tests/
libs_dir = libs/
components_dir = src/components/
error_dir = src/error/
COMPILER = gcc
FLAGS = -Wall -g

tests: tests.o libs.o components.o error.o
	$(COMPILER) $(FLAGS) -o tests *.o

tests.o: $(tests_dir)/*
	$(COMPILER) $(FLAGS) -c $(tests_dir)*.c -o tests.o

libs.o: $(libs_dir)/*
	$(COMPILER) $(FLAGS) -c $(libs_dir)*.c -o libs.o

components.o: $(components_dir)/*
	$(COMPILER) $(FLAGS) -c $(components_dir)Parser.c -o parser.o
	$(COMPILER) $(FLAGS) -c $(components_dir)Stripper.c -o stripper.o
	$(COMPILER) $(FLAGS) -c $(components_dir)HashMap.c -o hash_map.o

error.o: $(error_dir)/*
	$(COMPILER) $(FLAGS) -c $(error_dir)*.c -o error.o
clean:
	rm -f *.o
