#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "registers_commands_and_instructions.h"
#include "tables_data_structures.h"


/*	in this function we run on the current file that we are at	*/
boolean first_run( char *fName , line_Struct *line_struct_ptr , root * root_ptr , int *IC , int *DC, int num_of_rows )
{
	
	boolean Errors_Not_Found = TRUE;
	char file_name_read[BUFFER];		
	int line_number;
	FILE * fptr;
	
	sprintf(file_name_read ,"%s.%s" , fName , POST_MACRO_FILE);

	if(!(fptr = fopen(file_name_read,"r")))  
    {	printf("PROGRAM_ERROR :Fail to open file named by %s\n\n",file_name_read);
    	printf("let's check the next one\n");
   	 return FALSE;
    }
    
    
    
	for(line_number = 0 ; line_number < num_of_rows ; line_number++)
	{
		line_struct_initializer(&line_struct_ptr[line_number],(line_number+1));
		
		fgets(line_struct_ptr[line_number].lineContant, (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS) ,fptr);
		
		
		/*at this point we want to start breaking the string into words*/
		string_analyzer(&line_struct_ptr[line_number],line_number);
		
		if(  We_have_got_Error(  &line_struct_ptr[line_number]  )  )
		{	
			error_printer(&line_struct_ptr[line_number] , fName);
			Errors_Not_Found = FALSE;
			continue;
		}	
		
		/*here you check the parameters*/
		parameters_checker(&line_struct_ptr[line_number]);	
		
		if(  We_have_got_Error(  &line_struct_ptr[line_number]  )  )
		{
			error_printer(&line_struct_ptr[line_number] , fName);
			Errors_Not_Found = FALSE;
			continue;
		}	
			
		
		/*if we got over here it means that we havent fount any errors at this line , now we can add to the tables*/
		add_to_tables(&line_struct_ptr[line_number] , root_ptr , IC , DC );
		
		if(  We_have_got_Error(  &line_struct_ptr[line_number]  )  )
		{
			error_printer(&line_struct_ptr[line_number] , fName);
			Errors_Not_Found = FALSE;
			continue;
		}	
		
		
		
	}
	
	update_data_symbol_values(root_ptr , IC);
	
	if( (*IC + *DC) > MAX_CAPACITY_MEMORY_CELLS) /*if we pass the limitation of memory*/
	{
		printf("PROGRAM_ERROR: file named by %s,\npassed capacity of memory" ,fName );
		Errors_Not_Found = FALSE;
	}
	
	fclose(fptr);
 return Errors_Not_Found;
}

void error_printer( line_Struct * line_ptr , char * fName)
{
    printf("In file named %s in line number %d,\nThere is an Error defined as : \"%s\" \n\n" ,fName ,line_ptr -> line_number ,line_ptr -> lineError);
}

void update_data_symbol_values(root * root_ptr , int * IC) /*after we added all the symbols to our symbol table we update the values*/
{
	symbol_table_Node * symbol_runner = root_ptr -> root_symbol_table;
	
	while(symbol_runner != NULL)
	{
		if (symbol_runner -> word_is_Data)
		{
			symbol_runner -> Value = ( (symbol_runner -> Value) + *IC); /*at the end we update the value,base and offset*/
			symbol_runner -> symbol_offset = (symbol_runner -> Value) % 16;
			symbol_runner -> symbol_base = (symbol_runner -> Value) - (symbol_runner -> symbol_offset);
		}
		
		symbol_runner = symbol_runner -> next;
	}
}


