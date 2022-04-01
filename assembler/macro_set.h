#ifndef MACRO_SET_H
#define MACRO_SET_H

#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "registers_commands_and_instructions.h"
#include "tables_data_structures.h"

typedef struct root_macro_table
{
	struct macro_table * root_table;
	
}root_macro_table;

typedef struct macro_table
{
	boolean found_macro;
	
	char macro_name[MAX_LABEL_LENGTH];
	
	char macro_content[MAX_ROWS_MACRO][MAX_LINE_LENGTH];
	
	struct macro_table * next_macro;
	
} macro_table;



boolean macro_set(char *, int *);

void add_to_macro_table(root_macro_table *  , char * , FILE *  , char * );
boolean search_and_paste_macro( root_macro_table *  ,char * , int * , FILE *);

void macro_table_initializer(macro_table *);
void root_macro_table_initializer(root_macro_table * );

boolean empty_macro_table(root_macro_table * );
boolean empty_macro_line(macro_table * );

void deallocate_macro_list(root_macro_table * , char * );

#endif
