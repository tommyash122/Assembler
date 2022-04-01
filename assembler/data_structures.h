#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "Definitions.h"

typedef struct line_Struct
{
	/*line DATA*/
	int  line_number;
	int words_count_in_line;
	char lineContant[MAX_LINE_LENGTH + EXTRA_BUFFER_FOR_FGETS + 1];
	char lineError[MAX_ERROR_LENGTH];
	
	/*A matrix that will store our words separately*/
	char lineWords[MAX_LINE_LENGTH/2][MAX_LINE_LENGTH+1]; /* "MAX_LINE_LENGTH/2" = num of rows, Assuming that between words there is a comma or space*/
	
	
	/*label flags*/
	boolean word_first_is_label;
	boolean word_is_operand_label;
	
	
	/*instruction flags*/
	boolean word_is_Data;
	boolean word_is_String;
	boolean word_is_Entry;
	boolean word_is_Extern;
	
	
	/*command flags*/
	boolean word_is_command;
	boolean word_is_command_two_operands;
	boolean word_is_command_one_operands;
	boolean word_is_command_zero_operands;
	
	boolean word_is_source_immediate; 		/*Addressing method 0*/
	boolean word_is_source_direct;	 		/*Addressing method 1*/
	boolean word_is_source_index;	 		/*Addressing method 2*/
	boolean word_is_source_registerDirect; 	/*Addressing method 3*/
	
	boolean word_is_dest_immediate; 		/*Addressing method 0*/
 	boolean word_is_dest_direct;	  		/*Addressing method 1*/
	boolean word_is_dest_index;	  			/*Addressing method 2*/
	boolean word_is_dest_registerDirect;  	/*Addressing method 3*/
	
	/*exceptional flags*/
	boolean word_is_new;
	boolean word_is_error;
	boolean word_is_toIgnore;
	
} line_Struct;



boolean first_word_is_label(char * );
void which_instruction(char * ,line_Struct * );
void which_command(char * ,line_Struct * );
boolean word_is_valid_label(char * );
void first_word_checker(char * , line_Struct * );

void update_and_check_num_of_words(int , line_Struct * );
void update_error( line_Struct * ,char * );
boolean We_have_got_Error(line_Struct *);

boolean immediate_address(char *, line_Struct * );
boolean index_address(char * , line_Struct * );


#endif 
