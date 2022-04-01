#include "Definitions.h"
#include "data_structures.h"
#include "string_funcs.h"
#include "registers_commands_and_instructions.h"

/*the line data struct that will conatain the line itself , the line divided to words , and crucial flags*/
void line_struct_initializer(line_Struct * line_ptr,int line_num)
{

	int row;

	line_ptr -> line_number = line_num;
	line_ptr -> words_count_in_line = 0;
	memset(line_ptr -> lineContant 	, '\0' , MAX_LINE_LENGTH);
	memset(line_ptr -> lineError 	, '\0' , MAX_LINE_LENGTH);
	
	for (row=0 ; row < (MAX_LINE_LENGTH/2) ; row++)
		memset(line_ptr -> lineWords[row] , '\0' , MAX_LINE_LENGTH);
	
	line_ptr -> word_first_is_label = FALSE;
	line_ptr -> word_is_operand_label = FALSE;
	

    line_ptr -> word_is_Data = FALSE;
    line_ptr -> word_is_String = FALSE;
    line_ptr -> word_is_Extern = FALSE;
    line_ptr -> word_is_Entry = FALSE;

    line_ptr -> word_is_command = FALSE;
    line_ptr -> word_is_command_two_operands = FALSE;
	line_ptr -> word_is_command_one_operands = FALSE;
	line_ptr -> word_is_command_zero_operands = FALSE;

    line_ptr -> word_is_source_immediate = FALSE;
    line_ptr -> word_is_source_direct = FALSE;
    line_ptr -> word_is_source_index = FALSE;
    line_ptr -> word_is_source_registerDirect = FALSE;

    line_ptr -> word_is_dest_immediate = FALSE;
    line_ptr -> word_is_dest_direct = FALSE;
    line_ptr -> word_is_dest_index = FALSE;
    line_ptr -> word_is_dest_registerDirect = FALSE;
    
	line_ptr -> word_is_new = FALSE;
    line_ptr -> word_is_error = FALSE;
    line_ptr -> word_is_toIgnore = FALSE;
}





boolean first_word_is_label(char * runner)
{
	char tmp_copy[MAX_LABEL_LENGTH + BUFFER]; 
	char * tmp_copy_ptr = tmp_copy;
	int end = (strlen(runner) - 1);
	
	if(runner[end] != ':')
		return FALSE;
	
	memset(tmp_copy , '\0' , MAX_LABEL_LENGTH + BUFFER);
	tmp_copy_ptr = strncpy(tmp_copy , runner , end); /*we copy the previous string to a copy array for analyze*/
	
	if( (!word_is_valid_label(tmp_copy_ptr)) || (saved_word(tmp_copy_ptr)) )
		return FALSE;
		
	return TRUE;
}

void which_instruction(char * runner,line_Struct * line_ptr)
{

	if (strcmp(runner,".data") == 0)
	{
		line_ptr -> word_is_Data = TRUE;
	}
	else if (strcmp(runner,".string") == 0)
	{
		line_ptr -> word_is_String = TRUE;
	}
	else if (strcmp(runner,".entry") == 0)
	{
		line_ptr -> word_is_Entry = TRUE;
		
	}
	else /* (strcmp(runner,".extern") == 0)*/
	{
		line_ptr -> word_is_Extern = TRUE;
	}
	
		
	
		
}
void which_command(char * runner,line_Struct * line_ptr)
{
	if(TWO_operand(runner))
	{
		line_ptr -> word_is_command_two_operands = TRUE;
	}
	else if (ONE_operand(runner))
	{
		line_ptr -> word_is_command_one_operands = TRUE;
	}
	else /* (ZERO_operand(runner))*/
	{
		line_ptr -> word_is_command_zero_operands = TRUE;
	}
		
	
}


boolean word_is_valid_label(char * runner)
{
	int i;
	
	if(!isalpha(runner[0]) || (saved_word(runner)) || (strlen(runner)>MAX_LABEL_LENGTH) )
		return FALSE;
	
	for(i=1 ; i< (strlen(runner)) ; i++)
	{
		if(!isalnum(runner[i]))
			return FALSE;
		
	}
	
	return TRUE;
	
}


void first_word_checker(char * runner , line_Struct * line_ptr)
{
	
	if(word_is_instruction(runner))
	{
		which_instruction(runner,line_ptr); /*if the word we are looking at is : (.data/.string/.entry/.extern) */
	}
	
	else if(word_is_command(runner))
	{
		line_ptr -> word_is_command = TRUE;  /*if the word we are looking at is one of the 16 commands : add/prn/lea ... */
		
		which_command(runner,line_ptr);
	}	
	
	else if (!word_is_register(runner))
	{
		line_ptr -> word_is_new = TRUE; /*We came across a new word*/
		
		if(!word_is_valid_label(runner))
		{
			update_error(line_ptr , INVALID_WORD);
		}
	}

}





/*in this function we will check if we got a command and valid number of operands*/
void update_and_check_num_of_words(int operands_count , line_Struct * line_ptr )
{
	line_ptr -> words_count_in_line = operands_count--; /*after insert the value of num into our struct we subtruct 1 because of the first word*/

	if(line_ptr -> word_first_is_label)/*if we got a label at the start thats another one to subtruct*/
		operands_count--;
		
	if( (line_ptr -> word_is_command) && (operands_count != operands_required(line_ptr)) )
		update_error(line_ptr , INVALID_NUM_OPERANDS);
			
	else if ( (line_ptr -> word_is_String) && (operands_count != operands_required(line_ptr)) )
		update_error(line_ptr , INVALID_NUM_OPERANDS);
		
	else if ( (line_ptr -> word_is_Entry) && (operands_count != operands_required(line_ptr)) )
		update_error(line_ptr , INVALID_NUM_OPERANDS);
		
	else if ( (line_ptr -> word_is_Extern) && (operands_count != operands_required(line_ptr)) )
		update_error(line_ptr , INVALID_NUM_OPERANDS);
	
	else if ( (line_ptr -> word_is_Data) && (operands_count < MINIMUM_NUM_OF_PARAMETERS) )
		update_error(line_ptr , NO_PARAMETERS_RECEIVED);
}

void update_error( line_Struct * line_ptr , char * error_description)
{
	line_ptr -> word_is_error = TRUE;
	strcpy((line_ptr -> lineError) , error_description);
}

boolean We_have_got_Error(line_Struct * line_ptr)
{
	if(line_ptr -> word_is_error)
		return TRUE;
	return FALSE;
}





