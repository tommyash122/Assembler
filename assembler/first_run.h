#ifndef FIRST_RUN_H
#define FIRST_RUN_H

#include "Definitions.h"
#include "tables_data_structures.h"

boolean first_run(char *,line_Struct *, root * ,int *,int *,int );
void error_printer(line_Struct * ,char * );
void string_analyzer(line_Struct * , int );
void parameters_checker(line_Struct *  );
void add_to_tables(line_Struct *, root * , int * , int *);
void update_data_symbol_values(root * , int * );
void valid_operand(char * , line_Struct * , int );

#endif 
