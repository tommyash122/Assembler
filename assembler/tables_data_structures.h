#ifndef TABLES_DATA_STRUCTURES_H
#define TABLES_DATA_STRUCTURES_H

#include "data_structures.h"

/* symbols table */
typedef struct symbol_table_Node
{
	char symbol_Name[MAX_LABEL_LENGTH];
	char attribute[MAX_ATTRIBUTE_LENGTH]; /*code , data ,external*/
 

	int Value;

	boolean word_is_Data;
	boolean word_is_Entry;
	boolean word_is_Extern;
	
	unsigned int symbol_base;
	unsigned int symbol_offset;
	

	struct symbol_table_Node * next;
	
} symbol_table_Node;


/* image table */
typedef struct image_Node
{

	char attribute; /*A,R,E*/
	unsigned int binary;
	
	int Value;

	boolean word_is_direct_addressing;
	boolean word_is_direct_addressing_base;
	boolean word_is_direct_addressing_offset;
	
	char label_name[MAX_WORD_LENGTH];

	/*boolean isError;*/
	/*char errorDetails[];*/
	
	

	int lineNumber;

	struct image_Node * next;
	
} image_Node;

/* root pointers */
typedef struct root_pointers_structs
{
	line_Struct * root_line_struct;
	symbol_table_Node * root_symbol_table;
	image_Node * root_data_table;
	image_Node * root_code_table;
	
	int symbols_count;
	int data_count;
	int code_count;
	
} root;


/* coded line table */
typedef struct coded_line
{				
	
	unsigned int dest_address : 2 ;
	unsigned int dest_reg : 4 ;
	unsigned int source_address : 2 ;
	unsigned int source_reg : 4 ;
	unsigned int funct : 4 ;
	
	unsigned int : 0 ; 		/*force the next line*/
	
	unsigned int acsii_word : 16 ;
	
	
} coded_line;

void deallocate_list( root * ,char *);
void root_initializer(root * , line_Struct *);
void line_struct_initializer(line_Struct *,int );
void image_initializer(image_Node * );
void symbol_initializer(symbol_table_Node * );

int  convert_opcode(int );
void build_binary_word(line_Struct *, root * ,int * , unsigned int  );
void insert_word_to_array(coded_line *, int * );
unsigned int array_to_decimal(int * );

void get_command(char *, unsigned int * , unsigned int * );
void get_source(line_Struct *,unsigned int * );
void get_register(line_Struct * ,unsigned int *  , char *);

boolean empty_symbol_table(root * );
boolean empty_image_table(root * );
boolean empty_code_table(root * );


#endif
