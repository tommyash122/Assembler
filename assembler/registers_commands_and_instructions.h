#ifndef RESGISTERS_COMMANDS_AND_INSTRUCTIONS_H
#define RESGISTERS_COMMANDS_AND_INSTRUCTIONS_H

#include "Definitions.h"

#define REGISTERS_LIST_LENGTH 16
#define NUM_OF_COMMANDS_TWO_OPERANDS 5
#define NUM_OF_COMMANDS_ONE_OPERANDS 9
#define NUM_OF_COMMANDS_ZERO_OPERANDS 2
#define NUM_OF_INSTRUCTIONS 4

boolean word_is_register(char * );
boolean word_is_command(char * );
boolean TWO_operand(char * );
boolean ONE_operand(char * );
boolean ZERO_operand(char * );
boolean word_is_instruction(char * );
boolean saved_word(char * );
int operands_required(line_Struct * );


#endif
