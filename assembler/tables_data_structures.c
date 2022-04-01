#include "Definitions.h"
#include "data_structures.h"
#include "tables_data_structures.h"



void root_initializer(root * root_ptr , line_Struct * line_ptr)
{
	root_ptr -> root_line_struct = line_ptr;
	
	root_ptr -> root_symbol_table = NULL;
	root_ptr -> root_data_table = NULL;
	root_ptr -> root_code_table = NULL;
	
	root_ptr -> symbols_count = 0;
	root_ptr -> data_count = 0;
	root_ptr -> code_count = 0;
}

void image_initializer(image_Node * image_runner)
{

	image_runner -> word_is_direct_addressing = FALSE;
	image_runner -> word_is_direct_addressing_base = FALSE;
	image_runner -> word_is_direct_addressing_offset = FALSE;
	memset(image_runner -> label_name , '\0' , MAX_WORD_LENGTH);
	
	image_runner -> attribute = '\0';
	image_runner -> binary = 0;
	image_runner -> Value = 0;
	
	
	image_runner -> lineNumber = 0;
	image_runner -> next = NULL;
}

void symbol_initializer(symbol_table_Node * symbol_runner)
{
	memset(symbol_runner -> symbol_Name , '\0' , MAX_LABEL_LENGTH);
	memset(symbol_runner -> attribute , '\0' , MAX_ATTRIBUTE_LENGTH);
	
	symbol_runner -> Value = 0;
	
	symbol_runner -> word_is_Data = FALSE; 
	symbol_runner -> word_is_Entry = FALSE;
	symbol_runner -> word_is_Extern = FALSE;
	
	symbol_runner -> symbol_base = 0;
	symbol_runner -> symbol_offset = 0;
	
	
	symbol_runner -> next = NULL;
	
}

void deallocate_list( root * root_ptr , char * need_exit)
{
	symbol_table_Node * curr_symbol_node = root_ptr->root_symbol_table; /*head of tables*/
	image_Node * curr_data_node = root_ptr->root_data_table;
	image_Node * curr_code_node = root_ptr->root_code_table;
	
	symbol_table_Node * temp_symbol = curr_symbol_node;/*temporery holders of the nodes*/
	image_Node * temp_data = curr_data_node	;
	image_Node * temp_code = curr_code_node ;
	
	
	/*   free all the linked lists   */
	while(temp_symbol != NULL)	
	{
		curr_symbol_node = curr_symbol_node -> next;
		free(temp_symbol);
		temp_symbol = curr_symbol_node;
	}
	
	while(temp_data != NULL)
	{
		curr_data_node = curr_data_node -> next;
		free(temp_data);
		temp_data = curr_data_node;
	}
	
	while(temp_code != NULL)
	{
		curr_code_node = curr_code_node -> next;
		free(temp_code);
		temp_code = curr_code_node;
	}
	
	free(root_ptr -> root_line_struct); /*free the line struct that we initiate from the main function */
	free(root_ptr); /*free the root struct that points to all the data structures*/
	
	if(strcmp(need_exit , "exit") == 0)
		exit(0);
}

int convert_opcode(int opcode)
{
	int i;
	int decimal = 2;
	
	if(opcode == 0)
		return 1;
	
	for(i=1 ; i<opcode ; i++)
		decimal*=2;
		
	return decimal;
}

void get_source(line_Struct *line_ptr ,unsigned int * source_address )
{

	if(line_ptr -> word_is_source_immediate)
		*source_address = 0;
		
   	else if (line_ptr -> word_is_source_direct)
       	*source_address = 1;
        
   	else if (line_ptr -> word_is_source_index)
       	*source_address = 2;
        
   	else if (line_ptr -> word_is_source_registerDirect)
       	*source_address = 3;
   
}

void get_dest(line_Struct *line_ptr ,unsigned int * dest_address )
{

	if(line_ptr -> word_is_dest_immediate)
		*dest_address = 0;
		
   	else if (line_ptr -> word_is_dest_direct)
       	*dest_address = 1;
        
   	else if (line_ptr -> word_is_dest_index)
       	*dest_address = 2;
        
   	else if (line_ptr -> word_is_dest_registerDirect)
       	*dest_address = 3;
   
}

void get_register(line_Struct *line_ptr ,unsigned int * address_val , char * reg)
{
	int i;
	char * options[] = {"r0","r1","r2","r3","r4","r5","r6","r7","r8","r9","r10","r11","r12","r13","r14","r15" };
	
	for(i=0 ; i<16 ; i++)
	{	
		if(strcmp(reg , options[i]) == 0)
		{
			*address_val = i;
			return;
		}
	}
	
}

/*here we mainly build an array of ones '1' and zeros '0' to be coded into our wanted word*/
void build_binary_word(line_Struct *line_ptr,root * root_ptr ,int * bits_array , unsigned int Funct )
{
	int i = (line_ptr -> word_first_is_label)? 2 : 1 ;  /*if our first word is a label we start the count from 2 , otherwise from 1*/
	unsigned int destAddress = 0 , destReg = 0 , sourceAddress = 0 , sourceReg = 0 ;
	char register_name[REGISTER_MAX_LENGTH+1];
	char * runner;
	
	coded_line * code_array = (coded_line *)malloc(sizeof(coded_line));
	
	
	if(!code_array)
	{
		printf("PROGRAM_ERROR: memory allocation failed");
		deallocate_list(root_ptr ,EXIT);
	}
	

	
	if(line_ptr -> word_is_command_two_operands) /*if the source exist*/
	{
		get_source(line_ptr , &sourceAddress);	/*get the source*/
		
		if(line_ptr -> word_is_source_registerDirect) /*if we got a register*/
			get_register(line_ptr , &sourceReg , line_ptr -> lineWords[i]);
		
		else if (line_ptr -> word_is_source_index) /*if we got method 2 , we ne to separate the register from the label*/
		{
			memset(register_name , '\0' , REGISTER_MAX_LENGTH+1);
			runner = strchr(line_ptr -> lineWords[i] , '[');
			strncpy(register_name ,++runner , REGISTER_MAX_LENGTH );
			
			get_register(line_ptr , &sourceReg , register_name);	
		}	
	}
	
	if(line_ptr -> word_is_command_two_operands)
		i++;
	
	get_dest(line_ptr , &destAddress);
	
		
	if(line_ptr -> word_is_dest_registerDirect)
		get_register(line_ptr , &destReg , line_ptr -> lineWords[i]);
	
	else if (line_ptr -> word_is_dest_index)
	{
		memset(register_name , '\0' , REGISTER_MAX_LENGTH+1);
		runner = strchr(line_ptr -> lineWords[i] , '[');
		strncpy(register_name ,++runner , REGISTER_MAX_LENGTH );
		
		get_register(line_ptr , &destReg , register_name);	
	}
	
	code_array -> dest_address = destAddress; /*update the founded values*/
	code_array -> dest_reg = destReg;	
	code_array -> source_address = sourceAddress;	
	code_array -> source_reg = sourceReg;
	code_array -> funct = Funct;
	
	insert_word_to_array(code_array,bits_array);
	
	free(code_array);	
	
	
	
}

unsigned int array_to_decimal(int * bits_array)
{
	int i;
	unsigned int decimal = 0;
	
	for( i=0 ; i<16 ; i++)
	{
		decimal = decimal + ( pow(2,i) * bits_array[i] );
	}
	
	return decimal;
}

void insert_word_to_array(coded_line * code_array , int * bits_array)
{
	int i;
	unsigned int mask = 1;
	unsigned int curr_word;
	
	/*walking throgh the array and inserting the bit to it using bitwise operator (&)*/
	
	curr_word = code_array -> dest_address;
	
	for(i=0 ; i<2 ; i++)
	{
		bits_array[i] = (curr_word & mask);
		curr_word>>=1;
	}
	
	curr_word = code_array -> dest_reg;
	
	for(i=2 ; i<6 ; i++)
	{
		bits_array[i] = (curr_word & mask);
		curr_word>>=1;
	}
	
	curr_word = code_array -> source_address;
	
	for(i=6 ; i<8 ; i++)
	{
		bits_array[i] = (curr_word & mask);
		curr_word>>=1;
	}
	
	curr_word = code_array -> source_reg;
	
	for(i=8 ; i<12 ; i++)
	{
		bits_array[i] = (curr_word & mask);
		curr_word>>=1;
	}
	
	curr_word = code_array -> funct;
	
	for(i=12 ; i<16 ; i++)
	{
		bits_array[i] = (curr_word & mask);
		curr_word>>=1;
	}
	
}

void get_command(char * command ,unsigned int * opcode ,unsigned int * funct)
{
	if(strcmp(command, "mov") == 0)
	{
		*opcode = 0;
	}
	
	else if (strcmp(command, "cmp") == 0)
	{
        *opcode = 1;
	}
	
    else if (strcmp(command, "add") == 0)
    {
        *funct = 10;
        *opcode = 2;
    }

    else if (strcmp(command, "sub") == 0)
    {
        *funct = 11;
        *opcode = 2;
    }

    else if (strcmp(command, "lea") == 0)
    {
        *opcode = 4;
	}
	
    else if (strcmp(command, "clr") == 0)
    {
        *funct = 10;
        *opcode = 5;
    }

    else if (strcmp(command, "not") == 0)
    {
        *funct = 11;
        *opcode = 5;
    }

    else if (strcmp(command, "inc") == 0)
    {
        *funct = 12;
        *opcode = 5;
    }

    else if (strcmp(command, "dec") == 0)
    {
        *funct = 13;
        *opcode = 5;
    }

    else if (strcmp(command, "jmp") == 0)
    {
        *funct = 10;
        *opcode = 9;
    }

    else if (strcmp(command, "bne") == 0)
    {
        *funct = 11;
        *opcode = 9;
    }

    else if (strcmp(command, "jsr") == 0)
    {
        *funct = 12;
        *opcode = 9;
    }

    else if (strcmp(command, "red") == 0)
    {
        *opcode = 12;
	}
	
    else if (strcmp(command, "prn") == 0)
    {
        *opcode = 13;
	}
	
    else if (strcmp(command, "rts") == 0)
    {
        *opcode = 14;
	}
	
    else if (strcmp(command, "stop") == 0)
    {
        *opcode = 15;
    }
    
}


boolean empty_symbol_table(root * root_ptr)
{
	if(root_ptr -> root_symbol_table == NULL)
		return TRUE;
	return FALSE;
}

boolean empty_image_table(root * root_ptr)
{
	if(root_ptr -> root_data_table == NULL)
		return TRUE;
	return FALSE;
}

boolean empty_code_table(root * root_ptr)
{
	if(root_ptr -> root_code_table == NULL)
		return TRUE;
	return FALSE;
}
