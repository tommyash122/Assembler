#include "Definitions.h"
#include "data_structures.h"
#include "string_funcs.h"
#include "first_run.h"
#include "registers_commands_and_instructions.h"
#include "tables_data_structures.h"
#include "macro_set.h"

/*pre-first run step that sets the macro ,if exist*/
boolean macro_set(char * file_name , int * num_of_rows )
{
	
	char file_name_read[BUFFER];
	char file_name_write[BUFFER];
	char * line_buffer;
	char * curr_macro;
	
	
	root_macro_table * root; /*root of complete macro table data structure*/
	
	FILE * file_read;
	FILE * file_write;
	
	sprintf(file_name_read , "%s.%s" , file_name , INPUT_FILE); /*a name buffers to create new names of the file*/
	sprintf(file_name_write ,"%s.%s" , file_name , POST_MACRO_FILE);
	
	
	if( !(file_read = fopen(file_name_read , "r")) )
	{
		printf("PROGRAM_ERROR: Fail to open file named by %s\n\n",file_name_read);
    	printf("let's check the next one\n");
   	 return FALSE;
	}
	
	
	if( !(file_write = fopen(file_name_write , "w")) )
	{
		printf("PROGRAM_ERROR: Fail to open file named by %s\n\n",file_name_write);
    	printf("let's check the next one\n");
   	 return FALSE;
	}
	
	root = (root_macro_table *)malloc(sizeof(root_macro_table ));
	line_buffer = (char *)malloc( sizeof(char) * (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS + 1) );
	
	if((!root) || (!line_buffer))
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		free(line_buffer);
		deallocate_macro_list(root ,EXIT);
	}
	
	root_macro_table_initializer(root); /*initiate the root structure*/
	
	
	memset(line_buffer , '\0' , (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS + 1));
	
	/*we copy the current line from our file to our array buffer*/
	while( fgets(line_buffer, (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS) ,file_read) != NULL ) 
	{
		if( (curr_macro = (char *)(strstr(line_buffer , MACRO_START) != NULL)) ) /*if we found a macro declaration*/
		{	
			curr_macro = strtok(line_buffer , " \t\n"); /*we isolate the exect name word ofe macro*/
			curr_macro = strtok(NULL , " \t\n");
			curr_macro = AdvWhiteChars_both_sides(curr_macro);
			
			if(!saved_word(curr_macro))
			{
				add_to_macro_table(root , line_buffer , file_read ,curr_macro  );	/*we add it to our list struct*/
			}
			else
				printf("invalid macro name, a macro can't be with the same name as saved word...\n\n"); /*we Declare on the error*/
			continue;
			
		}
		
		curr_macro = AdvWhiteChars_both_sides(line_buffer);/*we check if thet line contains a macro*/
		
		if(!empty_macro_table(root))
		{  
			if(search_and_paste_macro(root,curr_macro,num_of_rows ,file_write)) /*if we found the wanted macro we paste it*/
				continue;
		}
		
		fprintf(file_write,"%s\n" , line_buffer);
		*num_of_rows = (*num_of_rows + 1);
		memset(line_buffer , '\0' , (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS + 1));
	}
	
	fclose(file_read);
	fclose(file_write);

	free(line_buffer);
	deallocate_macro_list(root , NO_EXIT );
	
	
 return TRUE;
}

boolean search_and_paste_macro( root_macro_table * root ,char * curr_macro , int *  num_of_rows , FILE * file_write)
{
	int row;
	
	macro_table * macro_table_runner = root -> root_table;
	
	while( macro_table_runner != NULL  )
	{
		if(strcmp(macro_table_runner -> macro_name , curr_macro ) == 0) /*search for macro name*/
			break;
		macro_table_runner = macro_table_runner -> next_macro;
	}
	
	if(macro_table_runner == NULL) /*final node check*/
		return FALSE;
	
	row = 0;
	
	/*if found, we paste the macro contant*/
	while( macro_table_runner -> macro_content[row][0] != '\0' )
	{	
		fprintf(file_write,"%s", macro_table_runner -> macro_content[row]);
		row++;
		*num_of_rows = (*num_of_rows + 1);
	}
	
	return TRUE;
	
}

void add_to_macro_table( root_macro_table * root , char * line_buffer , FILE * file_read , char * macroName )
{
	macro_table * new_macro_table_node = (macro_table *)malloc(sizeof(macro_table ));
	macro_table * macro_table_runner = root -> root_table;
	int row_number = 0;
	
	
	if(!new_macro_table_node)
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		deallocate_macro_list(root ,EXIT);
	}
	
	macro_table_initializer(new_macro_table_node); /*init a new macro head*/
	
	if(empty_macro_table(root)) /*if is is the first node that we insert*/
	{
		root -> root_table = new_macro_table_node;
		macro_table_runner = root -> root_table;
	}
	else
	{
		while( (macro_table_runner != NULL) && (macro_table_runner-> next_macro != NULL) ) /*else we advence until we reach the final node*/
			macro_table_runner = macro_table_runner -> next_macro;
			
		macro_table_runner -> next_macro = new_macro_table_node;
	}
	
	strcpy(new_macro_table_node -> macro_name , macroName); /*insert the macro name to the root of the table*/
	new_macro_table_node -> found_macro = TRUE; /*raise flag , found macro*/
	
	
	
	memset(line_buffer , '\0' , (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS + 1)); /*now we start to insert the lines the macro contains*/
	fgets(line_buffer, (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS) ,file_read);
	
	
	while( (strstr(line_buffer,MACRO_END) == NULL) && (row_number<MAX_ROWS_MACRO) ) /*while we are searching for the end of the macro*/
	{	
		strcpy(new_macro_table_node -> macro_content[row_number] , line_buffer);
		row_number++;	
		
		memset(line_buffer , '\0' , (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS + 1)); 
		fgets(line_buffer, (MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS) ,file_read);
		
	}
	
}


void macro_table_initializer(macro_table * macro_table_runner)
{
	int i;
	
	macro_table_runner -> found_macro = FALSE;
	
	memset(macro_table_runner->macro_name , '\0' , MAX_LABEL_LENGTH);
	
	for(i = 0 ; i< (MAX_ROWS_MACRO) ; i++)
		memset(macro_table_runner -> macro_content[i] , '\0' , MAX_LINE_LENGTH+1);
	
	macro_table_runner -> next_macro = NULL;
	
}


void root_macro_table_initializer(root_macro_table * root)
{
	root -> root_table = NULL;
}

void deallocate_macro_list(root_macro_table * root , char * need_exit)
{
	macro_table * table_runner = root -> root_table;
	macro_table * table_runner_temp = table_runner;
	
	while(table_runner_temp != NULL)
	{	
		table_runner = table_runner -> next_macro;
		free(table_runner_temp);
		table_runner_temp = table_runner;	
	}
	
	free(root);
	
	if(strcmp(need_exit , "exit") == 0)
		exit(0);
}

boolean empty_macro_table(root_macro_table * root)
{
	if(root -> root_table == NULL)
		return TRUE;
	return FALSE;
}


