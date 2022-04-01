#include "data_structures.h"
#include "first_run.h"
#include "registers_commands_and_instructions.h"
#include "tables_data_structures.h"
#include "output.h"

void create_output(char * file_name , root * root_of_tables )
{
	create_object(file_name, root_of_tables);
		
    create_entry(file_name, root_of_tables);
    	
    create_extern(file_name, root_of_tables);
	
}

void create_object(char * file_name, root * root_of_tables)
{
	FILE * file_runner;
	
	image_Node * data_runner = root_of_tables -> root_data_table;
	image_Node * code_runner = root_of_tables -> root_code_table;
	
	int mask = 0x000f; /*those masks will help us perform bitwise with the "&" operator only on the relevent bits and print it in hexa*/
	unsigned int binary_copy;
	unsigned int curr_B;
	unsigned int curr_C;
	unsigned int curr_D;
	unsigned int curr_E;
	char new_file_name[BUFFER]; 
	
	sprintf(new_file_name , "%s.%s" , file_name , OBJECT_FILE);
		
	
	if( !(file_runner = fopen(new_file_name , "w")) )
	{
		printf("PROGRAM_ERROR: Fail to open file named by %s\n\n",file_name);
       	printf("let's check the next one\n");
		deallocate_list(root_of_tables , NO_EXIT);
		return;
	}
	if( (root_of_tables->data_count == 0) && (root_of_tables->code_count == 0) )
		return;
	
	fprintf(file_runner,"\t\t%d %d\n", root_of_tables->code_count, root_of_tables->data_count); /*print the first line of IC and DC*/
	
	while(code_runner != NULL)
	{
	 	/*calculate the values using the right masks*/
		binary_copy = code_runner->binary;
		curr_E = (binary_copy) & (mask);
		binary_copy>>=4;
		curr_D = (binary_copy) & (mask);
		binary_copy>>=4;
		curr_C = (binary_copy) & (mask);
		binary_copy>>=4;
		curr_B = (binary_copy) & (mask);
		
		fprintf(file_runner,"%04d A%d-B%x-C%x-D%x-E%x\n" ,code_runner->Value
														 ,attribute_converter(code_runner) /*A part*/
														 ,curr_B
														 ,curr_C
														 ,curr_D
														 ,curr_E );
											 
		code_runner = code_runner -> next;									 
	}
	
	
	while(data_runner != NULL)
	{
		binary_copy = data_runner->binary;
		curr_E = (binary_copy) & (mask);
		binary_copy>>=4;
		curr_D = (binary_copy) & (mask);
		binary_copy>>=4;
		curr_C = (binary_copy) & (mask);
		binary_copy>>=4;
		curr_B = (binary_copy) & (mask);
		
		fprintf(file_runner,"%04d A%d-B%x-C%x-D%x-E%x\n" ,( (data_runner -> Value) + (INIT_MEMORY_ADDRESS) + (root_of_tables -> code_count) )
											 			 ,attribute_converter(data_runner) /*A part*/
														 ,curr_B
														 ,curr_C
														 ,curr_D
														 ,curr_E );
											 
		data_runner = data_runner -> next;
	}
	
	fclose(file_runner);
}

void create_entry(char * file_name, root * root_of_tables)
{
	FILE * file_runner;
	
	symbol_table_Node * symbol_runner = root_of_tables -> root_symbol_table;
	char new_file_name[BUFFER]; 
	
	sprintf(new_file_name , "%s.%s" , file_name , ENTRIES_FILE);
	
	
	if( !(file_runner = fopen(new_file_name , "w")) )
	{
		printf("PROGRAM_ERROR :Fail to open file named by %s\n\n",file_name);
       	printf("let's check the next one\n");
		deallocate_list(root_of_tables , NO_EXIT);
		return;
	}
	

	if(root_of_tables -> symbols_count == 0)
		return;
		
	while(symbol_runner != NULL)
	{
		if(symbol_runner -> word_is_Entry)
		{
			fprintf(file_runner,"%s,%d,%d\n" ,symbol_runner->symbol_Name
											   ,symbol_runner->symbol_base
											   ,symbol_runner->symbol_offset);
		}
			
		symbol_runner = symbol_runner -> next;
	}
		
	fclose(file_runner);
}

void create_extern(char * file_name, root * root_of_tables)
{
	FILE * file_runner;
	
	symbol_table_Node * symbol_runner = root_of_tables -> root_symbol_table;
	char new_file_name[BUFFER]; 
	
	sprintf(new_file_name , "%s.%s" , file_name , EXTERNALS_FILE);
	
	
	if( !(file_runner = fopen(new_file_name , "w")) )
	{
		printf("PROGRAM_ERROR :Fail to open file named by %s\n\n",file_name);
       	printf("let's check the next one\n");
		deallocate_list(root_of_tables , NO_EXIT);
		return;
	}
	

	if(root_of_tables -> symbols_count == 0)
		return;
		
	while(symbol_runner != NULL)
	{
		if(symbol_runner -> word_is_Extern)
		{
			fprintf(file_runner,"%s BASE %d\n%s OFFSET %d\n\n" ,symbol_runner->symbol_Name 
																 ,symbol_runner->symbol_base
																 ,symbol_runner->symbol_Name
														 		 ,symbol_runner->symbol_offset );
		}			
			
		symbol_runner = symbol_runner -> next;
	}
	
	fclose(file_runner);
}


int attribute_converter(image_Node * runner)
{
	if(runner -> attribute == ABSOLUTE)
		return 4;
	else if (runner -> attribute == RELOCATABLE)
		return 2;
	else /* (code_runner -> attribute == EXTERNAL)*/
		return 1;
}
