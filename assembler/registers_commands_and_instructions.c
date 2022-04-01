#include "Definitions.h"
#include "data_structures.h"
#include "string_funcs.h"
#include "registers_commands_and_instructions.h"



const char * registers[] = 
	{
		"r0",
		"r1",
		"r2",
		"r3",
		"r4",
		"r5",
		"r6",
		"r7",
		"r8",
		"r9",
		"r10",
		"r11",
		"r12",
		"r13",
		"r14",
		"r15"
	};

const char * commands_two_operands[] = 
	{
        "mov",
        "cmp",
        "add",
        "sub",
        "lea"
    };
    
const char *commands_one_operands[] = 
    {
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "jsr",
        "red",
        "prn"
    };
    
const char *commands_zero_operands[] =
    {
        "rts",
        "stop"
    };
    
    
const char *instructions[] =
    {
        ".data",
        ".string",
        ".entry",
        ".extern"
    };
        

boolean word_is_register(char *runner)  
{
    int i;
	
    for ( i = 0 ; i < ( REGISTERS_LIST_LENGTH ) ; i++ ) 
    {
        if (strcmp(registers[i], runner) == 0)
        {
            return TRUE;
        }
	}
    return FALSE;
}

boolean word_is_command(char *runner)
{

    if (TWO_operand(runner) || ONE_operand(runner) || ZERO_operand(runner))
        return TRUE;

    return FALSE;
}  

boolean TWO_operand(char *runner)
{
	int i;

    for ( i = 0 ; (i < NUM_OF_COMMANDS_TWO_OPERANDS) && (*(commands_two_operands[i]) != '\0') ; i++ )
    {
        if (strcmp(commands_two_operands[i], runner) == 0)
            return TRUE;
	}
    return FALSE;
} 
        
boolean ONE_operand(char *runner)
{
	int i;

    for ( i = 0 ; (i < NUM_OF_COMMANDS_ONE_OPERANDS) && (*(commands_one_operands[i]) != '\0') ; i++ )
    {
        if (strcmp(commands_one_operands[i], runner) == 0)
            return TRUE;
	}
    return FALSE;
} 

boolean ZERO_operand(char *runner)
{
	int i;

    for ( i = 0 ; (i < NUM_OF_COMMANDS_ZERO_OPERANDS) && (*(commands_zero_operands[i]) != '\0') ; i++ )
    {
        if (strcmp(commands_zero_operands[i], runner) == 0)
            return TRUE;
	}
    return FALSE;
}

boolean word_is_instruction(char *runner)
{
    int i;

    for ( i = 0 ; (i < NUM_OF_INSTRUCTIONS) && (*(instructions[i]) != '\0') ; i++ )
    {
        if (strcmp(instructions[i], runner) == 0)
            return TRUE;
	}
    return FALSE;
}

boolean saved_word(char * runner)
{
	if((word_is_instruction(runner)) || (word_is_command(runner)) || (word_is_register(runner)))
		return TRUE;
	return FALSE;
}

int operands_required(line_Struct * line_ptr)
{
	if(line_ptr -> word_is_command_zero_operands)
		return 0;
	if(line_ptr -> word_is_command_one_operands)
		return 1;
	if(line_ptr -> word_is_command_two_operands)
		return 2;
	if(line_ptr -> word_is_String)
		return 1;
	if(line_ptr -> word_is_Extern)
		return 1;
	if(line_ptr -> word_is_Entry)
		return 1;
		
		
	return -1; /*the function will never get here*/
	
	
}

         
        
        
        
