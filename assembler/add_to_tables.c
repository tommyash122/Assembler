#include "add_to_tables.h"
#include "Definitions.h"
#include "data_structures.h"
#include "tables_data_structures.h"


void add_to_tables(line_Struct *line_ptr , root * root_ptr , int *IC , int *DC)
{

	if (line_ptr -> word_is_Entry)/*we will check for the entries at the second run*/
		return;
	
	if( (line_ptr -> word_is_Data) || (line_ptr -> word_is_String) )
	{
		
		if(line_ptr -> word_first_is_label) /*we go and insert this label to pur symbol table*/
			add_to_symbol_table(line_ptr,root_ptr,IC,DC,DATA_ATTRIBUTE);
		
		add_to_image_table(line_ptr,root_ptr,IC,DC);
		
	}
	
	else if (line_ptr -> word_is_Extern)
		add_to_symbol_table(line_ptr,root_ptr,IC,DC,EXTERNAL_ATTRIBUTE);
	
		
	else if (line_ptr -> word_is_command)  	/*this line is command*/
	{
		
		if(line_ptr -> word_first_is_label) /*we go and insert this label to our symbol table*/
			add_to_symbol_table(line_ptr,root_ptr,IC,DC,CODE_ATTRIBUTE);
		
		add_to_image_table(line_ptr,root_ptr,IC,DC);
		
	}
			
}

void add_to_image_table(line_Struct *line_ptr , root * root_ptr , int *IC , int *DC )
{
	int i;
	int bits_array[CODED_WORD_LENGTH]; 	/*with the help of this array we are going to code the word*/
	unsigned int decimal;
	char attribute;
	unsigned int opcode = 0 , funct = 0 ;
	
	
	if(line_ptr -> word_is_Data)
		add_data_to_image_table(line_ptr , root_ptr , IC , DC);
		
	else if(line_ptr -> word_is_String)
		add_string_to_image_table(line_ptr , root_ptr , IC , DC);
	
	
	
	else
	{
		attribute = ABSOLUTE;
		memset(bits_array , 0 , CODED_WORD_LENGTH); /*set all cells in the array to 0 */
		
		
		i = (line_ptr -> word_first_is_label)? 1 : 0 ;
		
		
		get_command(line_ptr->lineWords[i] , &opcode , &funct); /*getting our opcode and funct (if exist)*/
		
		
		decimal = convert_opcode(opcode);
		
		
		/*adding the code of the opcode of our line to our table*/
		add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute  , decimal ,line_ptr->lineWords[i] ,  NO_DIRECT);
		
		
		if(line_ptr -> word_is_command_zero_operands)/*only one line to code when there is no operands*/
			return;
		
		
		build_binary_word(line_ptr, root_ptr , bits_array , funct);
		decimal = array_to_decimal(bits_array);
		
		
		/*adding the code of the addressing methods and registers of our line to our table*/
		add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute  , decimal ,line_ptr->lineWords[i] ,  NO_DIRECT);
		
		
		/*adding the code of the operands*/
		if(line_ptr -> word_is_command_two_operands)
		{
			i++;
			
			if(line_ptr -> word_is_source_immediate)
				add_immediate_code_to_image_table(line_ptr , root_ptr , IC , i);
				
			else if(line_ptr -> word_is_source_direct)
				add_direct_code_to_image_table(line_ptr , root_ptr , IC , i);
			
			else if(line_ptr -> word_is_source_index)
				add_index_code_to_image_table(line_ptr , root_ptr , IC , i);
				
			/*register Direct addressing method is defined without extra code word*/		
		}
		
		i++;
		
		if(line_ptr -> word_is_dest_immediate)
			add_immediate_code_to_image_table(line_ptr , root_ptr , IC , i);
				
		else if(line_ptr -> word_is_dest_direct)
			add_direct_code_to_image_table(line_ptr , root_ptr , IC , i);
			
		else if(line_ptr -> word_is_dest_index)
			add_index_code_to_image_table(line_ptr , root_ptr , IC , i);
		
	 }/*register Direct addressing method is defined without extra code word*/
				
		
}

void add_immediate_code_to_image_table(line_Struct *line_ptr , root * root_ptr ,int * IC ,int i)
{
	char attribute;
	int operand_in_decimal;
	char temp_copy[MAX_WORD_LENGTH];
	char * temp_copy_ptr = temp_copy;
	
	strcpy(temp_copy , line_ptr -> lineWords[i]);
	operand_in_decimal = atoi(++temp_copy_ptr);
	
	attribute = ABSOLUTE;
	
	add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute , operand_in_decimal , line_ptr -> lineWords[i] , NO_DIRECT );
		
}

void add_direct_code_to_image_table(line_Struct *line_ptr , root * root_ptr ,int * IC ,int i)
{

	/*at this point ,at the first run, we probably dont have enogth info about this label .
	so we just add empty code words to our table, save the label name and avence our IC*/
	char attribute = RELOCATABLE;
	
	add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute , (unsigned int )NULL , line_ptr -> lineWords[i] , DIRECT_BASE );
	
	add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute , (unsigned int )NULL , line_ptr -> lineWords[i] , DIRECT_OFFSET );
}

void add_index_code_to_image_table(line_Struct *line_ptr , root * root_ptr ,int * IC ,int i)
{
	char labelName[MAX_LABEL_LENGTH+1];
	char * runner;
	char * start = line_ptr -> lineWords[i];
	char attribute = RELOCATABLE;
	
	
	memset(labelName , '\0' , MAX_LABEL_LENGTH+1);
	runner = strchr(line_ptr -> lineWords[i] , '[');
	strncpy(labelName , start , runner-start );
	
	
	add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute , (unsigned int )NULL , labelName , DIRECT_BASE );
	
	add_code_node_to_image_table(line_ptr , root_ptr , IC , attribute , (unsigned int )NULL , labelName , DIRECT_OFFSET );
}




void add_code_node_to_image_table(line_Struct *line_ptr,  root * root_ptr , int *IC , char attribute , unsigned int decimal,char * word,char * method )
{
	image_Node * new_node = (image_Node *)malloc(sizeof(image_Node ));
	image_Node * runner = root_ptr -> root_code_table;
	
	if(!new_node)
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		deallocate_list(root_ptr,EXIT);
	}
	
	image_initializer(new_node);
	
	root_ptr -> code_count++;
	
	
	if(empty_code_table(root_ptr))
	{
		
		new_node -> attribute = attribute;
		new_node -> binary = decimal;
		new_node -> Value = *IC;
		new_node -> lineNumber = (line_ptr -> line_number);
	
		*IC =(*IC + 1);
	
		root_ptr -> root_code_table = new_node;
		
	 return;
	}
	
	
	while( (runner != NULL) && (runner -> next != NULL) )
		runner = runner -> next;
	
	new_node -> attribute = attribute;
	new_node -> binary = decimal;
	new_node -> Value = *IC;
	new_node -> lineNumber = (line_ptr -> line_number);
	
	*IC =(*IC + 1);
	
	runner -> next = new_node;
	
	if(strcmp(method , DIRECT_BASE) == 0)
	{
		new_node -> word_is_direct_addressing =TRUE;
		new_node -> word_is_direct_addressing_base =TRUE;
		strcpy(new_node -> label_name , word);
	}
	else if (strcmp(method , DIRECT_OFFSET) == 0)
	{
		new_node -> word_is_direct_addressing =TRUE;
		new_node -> word_is_direct_addressing_offset =TRUE;
		strcpy(new_node -> label_name , word);
	}
	
	
}



void add_string_to_image_table(line_Struct *line_ptr , root * root_ptr , int *IC , int *DC)
{
	int row;
	int start;
	int end;
	char attribute;
	
	
	coded_line * code_array = (coded_line *)malloc(sizeof(coded_line)) ;
	if(!code_array)
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		deallocate_list(root_ptr,EXIT);
	}
	
	attribute = ABSOLUTE ;
	
	row = (line_ptr -> word_first_is_label)? 2 : 1 ; /*if our first word is a label we start the count from 2 , otherwise from 1*/
	
	start = 1;
	end = strlen(line_ptr -> lineWords[row]) - 1;
	
	for( ; start<end ; start++)
	{
		code_array -> acsii_word = (line_ptr -> lineWords[row][start]);
		add_node_image_table( root_ptr , DC , attribute , code_array -> acsii_word);
		*DC = (*DC + 1) ;
	}
	
	code_array -> acsii_word = 0; /*at the end of the string we insert 0 be definition*/
	add_node_image_table( root_ptr , DC , attribute , code_array -> acsii_word);
	*DC = (*DC + 1) ;
	
	free(code_array);
}


void add_data_to_image_table(line_Struct * line_ptr, root * root_ptr, int * IC, int * DC)   /*word is data or string*/
{
	char attribute;
	int row;
	coded_line * code_array = (coded_line *)malloc(sizeof(coded_line)) ;
	
	if(!code_array)
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		deallocate_list(root_ptr,EXIT);
	}
	
	attribute = ABSOLUTE ;
	
	row = (line_ptr -> word_first_is_label)? 2 : 1 ; /*if our first word is a label we start the count from 2 , otherwise from 1*/
	
	for( ; row < (line_ptr -> words_count_in_line) ; row++ )
	{
		code_array -> acsii_word = atoi(line_ptr -> lineWords[row]);
		add_node_image_table( root_ptr , DC , attribute , code_array -> acsii_word);
		*DC = (*DC + 1) ;
	}
	
	free(code_array);
}

void add_node_image_table(root * root_ptr,int * DC ,char attribute , unsigned int code_array)
{
	image_Node * new_node = (image_Node *)malloc(sizeof(image_Node ));
	
	image_Node * runner  = root_ptr -> root_data_table;
	
	image_initializer(new_node);
	
	if(!new_node)
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		deallocate_list(root_ptr,EXIT);
	}
	
	root_ptr -> data_count++;
	
	if(empty_image_table(root_ptr))
	{
		add_first_node_image_table( root_ptr , DC , attribute , code_array , new_node);
	 return;
	}
	
	while( (runner != NULL) && (runner -> next != NULL) )
		runner = runner -> next;
	
	new_node -> attribute = attribute;
	new_node -> Value = *DC ;
	new_node -> binary = code_array ;
	
	runner -> next = new_node;
	
	
	
}

void add_first_node_image_table( root * root_ptr ,int * DC ,char attribute , unsigned int code_array ,  image_Node * new_node)
{
	new_node -> attribute = attribute;
	new_node -> Value = *DC ;
	new_node -> binary = code_array ;
	
	root_ptr -> root_data_table = new_node;
}



void add_to_symbol_table(line_Struct *line_ptr , root * root_ptr , int *IC , int *DC , char * attribute)
{
	char * label_name = line_ptr -> lineWords[0]; /*we entered here knowing that our first word in the line is a label*/
	
	symbol_table_Node * new_node = (symbol_table_Node *)malloc(sizeof(symbol_table_Node)); /*new_node is a pointer to a node in our symbol table*/
	
	symbol_table_Node * runner = root_ptr -> root_symbol_table; /*initiate a runner pointer to the start of our linked-list*/;
	
	symbol_initializer(new_node);
	
	if(!new_node) /*if we havent been able to allocte memory*/
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		free(new_node);
		deallocate_list(root_ptr,EXIT);
	}
	
	
	root_ptr -> symbols_count++; 
	
	if( (strcmp(attribute, EXTERNAL_ATTRIBUTE) == 0) ) /*check label before external declare*/
		label_name = line_ptr -> lineWords[1];
	
	
	if(empty_symbol_table(root_ptr)) 
	{
		add_first_node_symbol_table(line_ptr , root_ptr , IC , DC , new_node , label_name , attribute);
	 return;
	}
	
	
	while( (runner != NULL) && (runner -> next != NULL) ) /*we check if the label has seen so far*/
	{
		if(strcmp(runner -> symbol_Name , label_name) == 0)
		{
			update_error(line_ptr , LABEL_EXISTS);
		 return;
		}
		
		runner = runner -> next;	
	}
	
	if(strcmp(runner -> symbol_Name , label_name) == 0) /*at this point runner suppose to point to the last node in our linked list*/
	{
		update_error(line_ptr , LABEL_EXISTS);
	 return;
	}
	
	/*we have got here if we havent seen this label before*/
	runner -> next = new_node;
	
	
	strcpy(new_node -> symbol_Name , label_name); /*we insert the name of the current label that we are at to our node*/
	strcpy(new_node -> attribute , attribute);	  /*we insert the attribute of the current label that we are at to our node*/
	
	
	if (strcmp(attribute, DATA_ATTRIBUTE) == 0)
	{
		new_node -> word_is_Data = TRUE;
		new_node -> Value = *DC;
	}
	
	else if (strcmp(attribute, CODE_ATTRIBUTE) == 0)
	{
		new_node -> Value = *IC;
		new_node -> symbol_offset = (new_node -> Value) % 16;
		new_node -> symbol_base = (new_node -> Value) - (new_node -> symbol_offset);
		
	}
	
	else if (strcmp(attribute, EXTERNAL_ATTRIBUTE) == 0)
	{
		new_node -> word_is_Extern = TRUE;
		new_node -> Value = 0;
		
	}
	
	
	
}

void add_first_node_symbol_table(line_Struct *line_ptr ,root * root_ptr ,int *IC ,int *DC,
																			symbol_table_Node * new_node ,char * label_name ,char *attribute )
{
	
	root_ptr -> root_symbol_table = new_node;
	
	strcpy(new_node -> symbol_Name , label_name);
	strcpy(new_node -> attribute , attribute);
	
	
	if (strcmp(attribute, DATA_ATTRIBUTE) == 0)
	{
		new_node -> word_is_Data = TRUE;
		new_node -> Value = *DC;
	}
	
	else if (strcmp(attribute, CODE_ATTRIBUTE) == 0)
	{
		new_node -> Value = *IC;
	}
	
	else if (strcmp(attribute, EXTERNAL_ATTRIBUTE) == 0)
	{
		new_node -> word_is_Extern = TRUE;
		new_node -> Value = 0;
	}
		
	
}




