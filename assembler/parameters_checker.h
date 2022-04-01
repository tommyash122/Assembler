#ifndef PARAMETERS_CHECKER_H
#define PARAMETERS_CHECKER_H

#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "string_funcs.h"
#include "registers_commands_and_instructions.h"


void command_checker(char * , line_Struct * ,int);
boolean data_checker(char * , line_Struct * );
void string_checker(char * , line_Struct * );
void extern_checker(char * , line_Struct * );
void entry_checker(char * , line_Struct * );
void addressing_method_checker(char * , line_Struct * );


#endif
