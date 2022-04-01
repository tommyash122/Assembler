FLAGS = gcc -c -Wall -ansi -pedantic

ASSEMBLER_FLAGS = gcc -g -Wall -ansi -pedantic

OBJECTS = assembler.o\
data_structures.o\
first_run.o\
macro_set.o\
string_analyzer.o\
string_funcs.o\
tables_data_structures.o\
second_run.o\
add_to_tables.o\
registers_commands_and_instructions.o\
parameters_checker.o\
output.o\

HEADERS = data_structures.h\
Definitions.h\
first_run.h\
add_to_tables.h\
macro_set.h\
output.h\
parameters_checker.h\
registers_commands_and_instructions.h\
second_run.h\
string_funcs.h\
tables_data_structures.h

assembler: $(OBJECTS)
	$(ASSEMBLER_FLAGS) $(OBJECTS) -lm -o assembler

#assembler direct functions:
assembler.o: assembler.c $(HEADERS)
	$(FLAGS) assembler.c -lm -o assembler.o

macro_set.o: macro_set.c $(HEADERS)
	$(FLAGS) macro_set.c -lm -o macro_set.o
	
first_run.o: first_run.c $(HEADERS)
	$(FLAGS) first_run.c -lm -o first_run.o
	
second_run.o: second_run.c $(HEADERS)
	$(FLAGS) second_run.c -lm -o second_run.o

output.o: output.c $(HEADERS)
	$(FLAGS) output.c -lm -o output.o
	
#first run functions:
string_analyzer.o: string_analyzer.c $(HEADERS)
	$(FLAGS) string_analyzer.c -lm -o string_analyzer.o
	
parameters_checker.o: parameters_checker.c $(HEADERS)
	$(FLAGS) parameters_checker.c -lm -o parameters_checker.o
	
add_to_tables.o: add_to_tables.c $(HEADERS)	
	$(FLAGS) add_to_tables.c -lm -o add_to_tables.o
	
#data structures:
data_structures.o: data_structures.c $(HEADERS)
	$(FLAGS) data_structures.c -lm -o data_structures.o
	
tables_data_structures.o: tables_data_structures.c $(HEADERS)
	$(FLAGS) tables_data_structures.c -lm -o tables_data_structures.o
	
registers_commands_and_instructions.o: registers_commands_and_instructions.c $(HEADERS)
	$(FLAGS) registers_commands_and_instructions.c -lm -o registers_commands_and_instructions.o
	
#general functions:
string_funcs.o: string_funcs.c $(HEADERS)
	$(FLAGS) string_funcs.c -lm -o string_funcs.o
