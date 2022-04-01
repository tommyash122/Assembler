#include "Definitions.h"
#include "data_structures.h"
#include "string_funcs.h"
#include "registers_commands_and_instructions.h"
#include "parameters_checker.h"
#include "tables_data_structures.h"

void parameters_checker(line_Struct * line_ptr )
{
	int row;
	char * runner;
	char * command_copy_ptr;
	
	row = (line_ptr -> word_first_is_label)? 2 : 1 ; /*if our first word is a label we start the count from 2 , otherwise from 1*/
	
	/*a temporery pointer that holds the command instruction,if indeed we have a command line,it will be in use.*/
	command_copy_ptr = line_ptr -> lineWords[row - 1]; 
	
	/*runs on the line and sort each word to its right definition*/
	for( ; row <(line_ptr -> words_count_in_line) ; row++ )
	{
		runner = line_ptr -> lineWords[row];
		
		if(white_line(*runner))
		{
			update_error(line_ptr , UNEXPECTED_COMMA);
		 return;
		}
		
		if(line_ptr -> word_is_command)
		{
			command_checker(runner ,line_ptr,row);
			addressing_method_checker(command_copy_ptr , line_ptr);
		}	
		else if(line_ptr -> word_is_Data)
			data_checker(runner ,line_ptr );
		
		else if(line_ptr -> word_is_String)
			string_checker(runner ,line_ptr );
	
		else if(line_ptr -> word_is_Extern)
			extern_checker(runner ,line_ptr );
		
		else if(line_ptr -> word_is_Entry)
			entry_checker(runner ,line_ptr);
			
		else if(!word_is_register(runner))
			update_error(line_ptr , INVALID_WORD);
			
	}
		
 return;	
}

/*Here we are going to perform a syntax analysis of each operand*/


void command_checker(char * runner , line_Struct * line_ptr ,int operands_count)
{
	
	if( (((line_ptr -> words_count_in_line ) - 1 ) - operands_count)  ==  SOURCE) 
	 {
	 
	 	if(immediate_address(runner , line_ptr))
	 		line_ptr -> word_is_source_immediate = TRUE;
	 			
		else if(word_is_valid_label(runner))
		{
			line_ptr -> word_is_operand_label = TRUE;
			line_ptr -> word_is_source_direct = TRUE;
		}
			
		else if(word_is_register(runner))
			line_ptr -> word_is_source_registerDirect = TRUE;
				
		else if(index_address(runner , line_ptr))
		{
			line_ptr -> word_is_operand_label = TRUE;
			line_ptr -> word_is_source_index = TRUE;
		}		
		else
			update_error(line_ptr , INVALID_WORD);
	 return;		
			
	}	
			
	else if( (((line_ptr -> words_count_in_line ) - 1 ) - operands_count)  ==  DEST)
	{	
	
		if(immediate_address(runner , line_ptr))
	 		line_ptr -> word_is_dest_immediate = TRUE;
	 			
		else if(word_is_valid_label(runner))
		{
			line_ptr -> word_is_operand_label = TRUE;
			line_ptr -> word_is_dest_direct = TRUE;
		}
			
		else if(word_is_register(runner))
			line_ptr -> word_is_dest_registerDirect = TRUE;
				
		else if(index_address(runner , line_ptr))
		{
			line_ptr -> word_is_operand_label = TRUE;
			line_ptr -> word_is_dest_index = TRUE;
		}	
		else
			update_error(line_ptr , INVALID_WORD);
	 return;
		
	}
	
	else
		update_error(line_ptr , INVALID_NUM_OPERANDS);
		
		
 return;
}



boolean data_checker(char * head , line_Struct * line_ptr)
{
	int i = 0;
	
	
	if(signed_integer(head[i]))
		i++;
	
	for( ; i < strlen(head) ; i++)
	{
		if( !isdigit(head[i]) )
		{
			update_error(line_ptr , EXPECTED_DIGIT);
			return FALSE;
		}
	}
 return TRUE;
}

void string_checker(char * head , line_Struct * line_ptr)
{
	int end = (strlen(head) - 1);
	int start = 0;
	char tmp_copy[MAX_WORD_LENGTH];
	memset(tmp_copy , '\0' , MAX_WORD_LENGTH);
	
	strcpy(tmp_copy , head);
	
	
	if( (tmp_copy[start] != '"') || (tmp_copy[end] != '"') )
		update_error(line_ptr , INVALID_STRING);
	
	
	for(start = 1 ; start < end ; start++)
	{
		if((tmp_copy[start] == '"') && (tmp_copy[start - 1] != '\\'))
			update_error(line_ptr , INVALID_STRING);
	}
	
 return;
}

void extern_checker(char * runner , line_Struct * line_ptr)
{

	if((word_is_instruction(runner)) || (word_is_command(runner)) || (word_is_register(runner)))
		update_error(line_ptr , EXPECTED_LABEL);

	
	else if (!word_is_valid_label(runner))
		update_error(line_ptr , INVALID_WORD);

	
	line_ptr -> word_is_toIgnore = TRUE;    
	
 return;
}

void entry_checker(char * runner , line_Struct * line_ptr)
{

	if((word_is_instruction(runner)) || (word_is_command(runner)) || (word_is_register(runner)))
		update_error(line_ptr , EXPECTED_LABEL);
		
		
	else if (!word_is_valid_label(runner))
		update_error(line_ptr , INVALID_WORD);
		
	line_ptr -> word_is_toIgnore = TRUE;
	
 return;
}

/*immediate address method 0*/
boolean immediate_address(char * head , line_Struct * line_ptr)
{

	if(head[0] != '#')
		return FALSE;
	return data_checker(++head ,line_ptr);
	
}

/*index address method 2*/
boolean index_address(char * head, line_Struct * line_ptr)
{
	int i;
	char * runner;
	char * options[] = {"[r10]" , "[r11]" , "[r12]" , "[r13]" , "[r14]" , "[r15]"};
	char temp_copy[MAX_LABEL_LENGTH + BUFFER];
	memset(temp_copy , '\0' , MAX_LABEL_LENGTH + BUFFER );
	
	for( i=0 ; i<6 ; i++ )
	{
		if( (runner = strstr(head , options[i])) != NULL)
		{
			if(!word_is_valid_label(strncpy(temp_copy , head , (runner - head))))
				update_error(line_ptr , INVALID_WORD);
			return TRUE;
		}		
	}
	return FALSE;		
			
			
}

/*checks if commans fit to the operands*/
void addressing_method_checker(char * runner , line_Struct * line_ptr)
{
	if ( (strcmp( runner , "mov" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "add" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "sub" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "lea" ) == 0) && (  (line_ptr -> word_is_dest_immediate) || 
										   	 (line_ptr -> word_is_source_immediate) || (line_ptr -> word_is_source_registerDirect)  ) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "clr" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "not" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "inc" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "dec" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "jmp" ) == 0) && (  (line_ptr -> word_is_dest_immediate) || (line_ptr -> word_is_dest_registerDirect)  ) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "bne" ) == 0) && (  (line_ptr -> word_is_dest_immediate) || (line_ptr -> word_is_dest_registerDirect)  ) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "jsr" ) == 0) && (  (line_ptr -> word_is_dest_immediate) || (line_ptr -> word_is_dest_registerDirect)  ) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
		
	else if ( (strcmp( runner , "red" ) == 0) && (line_ptr -> word_is_dest_immediate) )
		update_error(line_ptr , INVALID_ADDRESSING_METHOD);
	
}


