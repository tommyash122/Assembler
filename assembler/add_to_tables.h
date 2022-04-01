#ifndef ADD_TO_TABLES_H
#define ADD_TO_TABLES_H

#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "tables_data_structures.h"


void add_to_image_table(line_Struct * , root *  , int * , int * );
void add_data_to_image_table(line_Struct * , root * , int * , int * );
void add_string_to_image_table(line_Struct * , root * , int * , int * );
void add_node_image_table( root * ,int * ,char , unsigned int );
void add_first_node_image_table( root *,int * ,char , unsigned int , image_Node * );

void add_code_node_to_image_table(line_Struct *,  root *  , int * , char, unsigned int ,char *,char * );
void add_immediate_code_to_image_table(line_Struct * , root *  ,int * ,int );
void add_direct_code_to_image_table(line_Struct * , root *  ,int * ,int );
void add_index_code_to_image_table(line_Struct * , root *  ,int * ,int );

void add_to_symbol_table(line_Struct * , root * , int * , int * , char * );
void add_first_node_symbol_table(line_Struct *,root * ,int *,int *,symbol_table_Node *,char *,char *);


#endif
