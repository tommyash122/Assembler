#include "second_run.h"
#include "first_run.h"
#include "Definitions.h"
#include "data_structures.h"
#include "tables_data_structures.h"


boolean second_run(char *fName , line_Struct *line_struct_ptr ,  root * root_ptr , int num_of_rows )
{
	boolean Errors_Not_Found = TRUE;
	char file_name_read[BUFFER];
	int line_number;
	FILE * fptr;
	
	symbol_table_Node * symbol_runner = root_ptr -> root_symbol_table;
	image_Node * code_runner = root_ptr -> root_code_table;
	
	sprintf(file_name_read ,"%s.%s" , fName , POST_MACRO_FILE);

	if(!(fptr = fopen(file_name_read,"r")))  
    {	printf("PROGRAM_ERROR: Fail to open file named by %s\n\n",file_name_read);
    	printf("let's check the next one\n");
   	 return FALSE;
    }
    
    
    
    for(line_number = 0 ; line_number < num_of_rows ; line_number++)
    {
    	
    	if(line_struct_ptr[line_number].word_is_Entry) /*line that contains an entry declaretion*/
    		search_entry(&line_struct_ptr[line_number] ,symbol_runner);
    	
    	if(  We_have_got_Error(  &line_struct_ptr[line_number]  )  )
		{
			error_printer(&line_struct_ptr[line_number] , fName);
			Errors_Not_Found = FALSE;
			continue;
		}
    	
    	
    	/*in that case this line was fully coded already*/
    	if( !line_struct_ptr[line_number].word_is_operand_label )
    		continue;
    		
    	/*otherwise we check that line*/	
    	string_second_analayzer(&line_struct_ptr[line_number] , symbol_runner, code_runner );
 
    	if(  We_have_got_Error(  &line_struct_ptr[line_number]  )  )
		{
			error_printer(&line_struct_ptr[line_number] , fName);
			Errors_Not_Found = FALSE;
			continue;
		}
    			
    	
    }
    
    fclose(fptr);
  return Errors_Not_Found; 
    
}

void search_entry(line_Struct * line_ptr , symbol_table_Node * symbol_runner)
{
	int i = (line_ptr -> word_first_is_label)? 2 : 1 ; /*if our first word is a label we start the count from 2 , otherwise from 1*/ 
	char * entry_label_name = line_ptr -> lineWords[i];
	
	while(symbol_runner != NULL)
	{
		if(strcmp(entry_label_name , symbol_runner -> symbol_Name) == 0) /*search in our symbol linked list for a match with the label names */
		{
			
			if(symbol_runner -> word_is_Entry) /*if this is true already, it means that , this entry label was already declared */
			{
				update_error(line_ptr , MULTIPLE_DECLARATIONS_ENTRY);
			 return;
			}
			
			symbol_runner -> word_is_Entry =TRUE; /*WE FOUND THE WANTED ENTRY LABEL ,now we update the entry flag in the symbol struct*/
			break;
		}
		
		symbol_runner = symbol_runner -> next;
	}
	
	if(symbol_runner == NULL) /*we enter here if we havent found a label's definition*/
	{
		update_error(line_ptr , UNDEFINED_ENTRY_LABEL);
	 return;
	}
	
	
}

void string_second_analayzer(line_Struct * line_ptr , symbol_table_Node * symbol_runner,image_Node * code_runner )
{
	
	symbol_table_Node * head_symbol_table = symbol_runner;
	
	int i = (line_ptr -> word_first_is_label)? 2 : 1 ; /*if our first word is a label we start the count from 2 , otherwise from 1*/
	char labelName[MAX_LABEL_LENGTH+1];
	char * runner;
	char * label_Name_ptr = line_ptr -> lineWords[i];
	
	
	if(source_operand_is_label(line_ptr)) /*Examining the first operand*/
	{
		if(line_ptr -> word_is_source_index)
		{
			memset(labelName , '\0' , MAX_LABEL_LENGTH+1);
			runner = strchr(label_Name_ptr , '[');
			label_Name_ptr = strncpy(labelName , label_Name_ptr , runner-label_Name_ptr );	
		}
		
		while(symbol_runner != NULL)
		{
			if(strcmp(label_Name_ptr , symbol_runner -> symbol_Name) == 0) /*search in our linked list for a match with the label names */
			{
				/*if found , update the line codes of base and offset*/
				update_base_offset_attribute(line_ptr,symbol_runner,code_runner,label_Name_ptr,symbol_runner -> attribute); 
				break;
			}
			
			symbol_runner = symbol_runner -> next;
		}
		
		if(symbol_runner == NULL) /*if we found an Error in this line we can return*/
		{
			update_error(line_ptr , UNDEFINED_LABEL);
		 return;
		}
		
	 
	}
	
	if(line_ptr -> word_is_command_two_operands) /*if there is one more operand to check*/
		label_Name_ptr = line_ptr -> lineWords[++i]; 
	 								
	symbol_runner = head_symbol_table; /*returning the runner pointer to the head of the symbol linked list*/
	
	
	if(dest_operand_is_label(line_ptr)) /*Examining the second operand*/
	{
	
		if(line_ptr -> word_is_dest_index)
		{
			memset(labelName , '\0' , MAX_LABEL_LENGTH+1);
			runner = strchr(label_Name_ptr , '[');
			label_Name_ptr = strncpy(labelName , label_Name_ptr , runner-label_Name_ptr );	
		}
		
		while(symbol_runner != NULL)
		{
			if(strcmp(label_Name_ptr , symbol_runner -> symbol_Name) == 0)
			{
				update_base_offset_attribute(line_ptr,symbol_runner , code_runner ,label_Name_ptr,symbol_runner -> attribute );
				break;
			}
			
			symbol_runner = symbol_runner -> next;
		}
		
		if(symbol_runner == NULL)
		{
			update_error(line_ptr , UNDEFINED_LABEL);
		 return;
		}
			
	}
		
	
}

void update_base_offset_attribute(line_Struct* line_ptr,symbol_table_Node* symbol_runner,image_Node* code_runner,char* label_Name_ptr,char* attribute)
{
	boolean found_base = FALSE;
	boolean found_offset = FALSE;

	unsigned int curr_value = symbol_runner -> Value;
	unsigned int base;
	unsigned int offset;
	char curr_attribute;
	
	/*calculate the base and offset values*/
	offset = curr_value % 16;
	base = curr_value - offset;
	
	
	if( (strcmp(attribute , EXTERNAL_ATTRIBUTE) == 0) )
		curr_attribute = EXTERNAL;
	else
		curr_attribute = RELOCATABLE;
	
	
	while(code_runner != NULL)
	{
		/*update the base code of the label operand*/
		if( (strcmp(label_Name_ptr , code_runner -> label_name) == 0) && (code_runner -> word_is_direct_addressing_base) )
		{
			code_runner -> attribute = curr_attribute;
			code_runner -> binary = base;
			if(symbol_runner->word_is_Extern)
				symbol_runner -> symbol_base = (code_runner -> Value);
				
			found_base = TRUE;
		}
		/*update the offset code of the label operand*/
		if( (strcmp(label_Name_ptr , code_runner -> label_name) == 0) && (code_runner -> word_is_direct_addressing_offset) )
		{
			code_runner -> attribute = curr_attribute;
			code_runner -> binary = offset;
			if(symbol_runner->word_is_Extern)
				symbol_runner -> symbol_offset = (code_runner -> Value);
				
			found_offset = TRUE;
		}
		
		code_runner = code_runner -> next;
	}
	
	if(!found_base || !found_offset) /*if we didnt found both of the code lines necessary we update an Error and return*/
	{
		update_error(line_ptr , UNDEFINED_LABEL);
	}
	return;
}

boolean source_operand_is_label(line_Struct * line_ptr)
{
	if( (line_ptr -> word_is_operand_label) && ( (line_ptr -> word_is_source_direct) || (line_ptr -> word_is_source_index) ) )
		return TRUE;
	return FALSE;
}

boolean dest_operand_is_label(line_Struct * line_ptr)
{
	if( (line_ptr -> word_is_operand_label) && ( (line_ptr -> word_is_dest_direct) || (line_ptr -> word_is_dest_index) ) )
		return TRUE;
	return FALSE;
}

