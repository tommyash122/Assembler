#ifndef OUTPUT_H
#define OUTPUT_H

#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "registers_commands_and_instructions.h"
#include "tables_data_structures.h"

void create_output(char * , root * );
void create_object(char * , root * );
void create_entry(char * , root * );
void create_extern(char * , root *);

int attribute_converter(image_Node * );


#endif
