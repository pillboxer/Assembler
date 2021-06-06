tests_dir = src/tests/*
libs_dir = libs/*
components_dir = src/components/

COMPILER = gcc
FLAGS = -Wall -g

tests: tests.o libs.o components.o
	$(COMPILER) $(FLAGS) -o tests *.o

tests.o: $(tests_dir).c $(tests_dir).h
	$(COMPILER) $(FLAGS) -c $(tests_dir).c -o tests.o

libs.o:
	$(COMPILER) $(FLAGS) -c $(libs_dir).c -o libs.o

components.o: $(components_dir)/*
	$(COMPILER) $(FLAGS) -c $(components_dir)/Stripper.c -o stripper.o
	$(COMPILER) $(FLAGS) -c $(components_dir)/Parser.c -o parser.o
clean:
	rm -f *.o
