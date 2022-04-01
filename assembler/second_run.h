#ifndef SECOND_RUN_H
#define SECOND_RUN_H

#include "Definitions.h"
#include "data_structures.h"
#include "tables_data_structures.h"

boolean second_run(char * , line_Struct * ,  root * , int  );

void extern_search(line_Struct *  ,symbol_table_Node *, char * );
void string_second_analayzer(line_Struct *  , symbol_table_Node * ,image_Node * );
void update_base_offset_attribute(line_Struct *  , symbol_table_Node * ,image_Node * ,char * ,char * );
void search_entry(line_Struct * , symbol_table_Node *);

boolean source_operand_is_label(line_Struct * );
boolean dest_operand_is_label(line_Struct * );


#endif
