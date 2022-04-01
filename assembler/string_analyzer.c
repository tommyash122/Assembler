#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "string_funcs.h"
#include "registers_commands_and_instructions.h"


void string_analyzer(line_Struct * line_ptr, int line_number)
{
	int    word_number = 0;
	char * first_sub_string = NULL;
	char   curr_str[MAX_LINE_LENGTH+1]; /*copy of the line*/
	char * curr_str_head = (line_ptr -> lineContant);
	char * Parameter_ptr = NULL;
	
	
	if(!valid_length(curr_str_head))
	{
		update_error(line_ptr , INVALID_LENGTH);
	 return;
	}
	
	curr_str_head = AdvWhiteChars_both_sides(curr_str_head); /*we skip white chars from the left and right side of the string*/
	
	
	if(line_is_comment(curr_str_head) || white_line(*curr_str_head))
	{
		line_ptr -> word_is_toIgnore = TRUE;
	 return;
	}
	
	
	strcpy(curr_str,curr_str_head);/*copy the line from out line structure to a local array for analyze*/
	
	
	if(!valid_last_char( curr_str[strlen(curr_str) - 1] ))
	{
		update_error(line_ptr , EXTRANEOUS_TEXT);
	 return;
	}
	
	if(strstr(curr_str , ",,")) /*if this sub-string exist in our line , we update an error line and return*/
	{
		update_error(line_ptr , UNEXPECTED_COMMA);
	 return; 
	}
	
	first_sub_string  = strtok(curr_str," \t\n");/*now we hold pointer to our first word in the line*/
	
	
	/*if our first word is a label*/
	if( first_word_is_label(first_sub_string) ) 
	{
		
		line_ptr -> word_first_is_label = TRUE;

		
		/*we will remove the ':' char from the label and insert it to lineWords that stores out words separately*/
		strncpy(line_ptr -> lineWords[word_number++],first_sub_string,(strlen(first_sub_string)-1)); 
		
		
		first_sub_string = strtok(NULL," \t\n"); /*advence our pointer to the next word*/
		
	}
	
	
	/*categorize our current word */
	first_word_checker(first_sub_string,line_ptr);
	
	if(We_have_got_Error(line_ptr))
		return;
	
	/*A special case when a label defined before an entry/extern declaration*/
	if( (line_ptr -> word_first_is_label) && ( (line_ptr -> word_is_Extern) || (line_ptr -> word_is_Entry) ) )
	{
		puts(LABEL_BEFORE_EXTERN_ENTRY);
		printf("In line number:%d, label name:%s\n\n" , (line_number+1) , line_ptr -> lineWords[(word_number = 0)]);
		
		line_ptr -> word_first_is_label = FALSE;
	
		memset(line_ptr -> lineWords[word_number] , '\0' , MAX_LINE_LENGTH);
		
		
	}
	
	
	/*insert the current word we are at*/
	strcpy(line_ptr -> lineWords[word_number++],first_sub_string); 
	
	
	/*...after we have done with the first part we continue to the operands analysis...*/
	
	
	/*right now we have advenced our string to the point we haven't Process yet , the parameters*/
	Parameter_ptr = strcpy( curr_str, curr_str_head + ( (first_sub_string-curr_str) + strlen(first_sub_string)) );
	
	Parameter_ptr = AdvWhiteChars_both_sides(Parameter_ptr); /*we go over the white chars after the first word*/
	
	
	if(unexpected_comma(*Parameter_ptr)) /*after an instruction or commnad we are not expecting a comma*/
	{
		update_error(line_ptr , UNEXPECTED_COMMA);
	 return;
	}
	
	Parameter_ptr =strtok(Parameter_ptr , ",");
	
	while(Parameter_ptr != NULL)
	{
		Parameter_ptr = AdvWhiteChars_both_sides(Parameter_ptr);	

		strcpy(line_ptr -> lineWords[word_number++],Parameter_ptr);
		
		Parameter_ptr =strtok(NULL , ",");
		
	}
	
	/*here we make sure that we got a valid number of operands that matches to our requirement*/
	update_and_check_num_of_words(word_number , line_ptr ); 

 return;
}


