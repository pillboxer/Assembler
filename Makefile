tests_dir = src/tests/
libs_dir = libs/
components_dir = src/components/
error_dir = src/error/
COMPILER = gcc
FLAGS = -Wall -g

assembler: assembler.o components.o libs.o  error.o
	$(COMPILER) $(FLAGS) -o assembler *.o

assembler.o: src/assembler.c
	$(COMPILER) $(FLAGS) -c src/assembler.c -o assembler.o

tests_add: tests

tests_max: FLAGS += -DMAX
tests_max: tests

tests_rect: FLAGS += -DRECT
tests_rect: tests

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
	rm tests
	rm -f *.o
