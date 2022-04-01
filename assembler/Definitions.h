#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef enum 
{
	FALSE = 0 , 
	TRUE = 1
} boolean;


/* Lengths: */
#define VALID_NUMBER_OF_ARGS 2
#define MAX_LINE_LENGTH 81 /* Max line length */
#define EXTRA_BUFFER_FOR_FGETS 80
#define MAX_LABEL_LENGTH 31
#define MAX_ATTRIBUTE_LENGTH 10
#define MAX_LINES_TO_BE_CODED 6
#define MAX_WORD_LENGTH 80
#define MAX_ATTRIBUTES_PER_SYMBOL 1
#define MAX_ERROR_LENGTH 30
#define INVALID_WORD_LENGTH 0
#define BUFFER 31
#define MINIMUM_NUM_OF_PARAMETERS 1
#define CODED_WORD_LENGTH 16
#define REGISTER_MAX_LENGTH 3
#define MAX_ROWS_MACRO 6
#define SOURCE 1
#define DEST 0 
#define MAX_CAPACITY_MEMORY_CELLS 8192


/* Addresses: */
#define INIT_MEMORY_ADDRESS 100
#define INIT_MEMORY_LABEL_ADDRESS 0


/* Symbols */
#define ABSOLUTE 'A'
#define RELOCATABLE 'R'
#define EXTERNAL 'E'


/* Labels Attributes */
#define DATA_ATTRIBUTE "data"
#define CODE_ATTRIBUTE "code"
#define EXTERNAL_ATTRIBUTE "external"


/* Attention addressing method */
#define NO_DIRECT "noDirect"
#define DIRECT_BASE "DirectBase"
#define DIRECT_OFFSET "DirectOffset"

/* actions*/
#define EXIT "exit"
#define NO_EXIT "no exit"
#define FINAL_OUTPUT "final output"
#define POST_MACROS "post macros"
#define MACRO_START "macro"
#define MACRO_END "endm"

/* file name suffix */
#define INPUT_FILE "as"
#define POST_MACRO_FILE "am"
#define OBJECT_FILE "ob"
#define ENTRIES_FILE "ent"
#define EXTERNALS_FILE "ext"

/* Common error descriptions */
#define UNEXPECTED_COMMA "Unexpected comma received"
#define EXTRANEOUS_TEXT "Extraneous text"
#define EXTRANEOUS_COMMA "Extraneous comma"
#define INVALID_WORD "invalid word received"
#define EXPECTED_DIGIT "expected digit"
#define INVALID_STRING "invalid string received"
#define EXPECTED_LABEL "EXPECTED_LABEL"
#define INVALID_NUM_OPERANDS "invalid number of operands received"
#define NO_PARAMETERS_RECEIVED "no parameters received"
#define INVALID_LENGTH "invalid length of line received"
#define INVALID_ADDRESSING_METHOD "invalid addressing method"
#define LABEL_EXISTS "Multiple definitions of the same label OR external label defined localy"
#define EXTERN_DEFINITION "external label defined localy"
#define UNDEFINED_LABEL "undefined label"
#define UNDEFINED_ENTRY_LABEL "entry label declared but not defined"
#define MULTIPLE_DECLARATIONS_ENTRY "multiple declarations of the same entry label"
#define INVALID_FILE_NAME_SUFFIX "input file must end with the .as suffix , can't open this kind of file"
#define LABEL_BEFORE_EXTERN_ENTRY "WARNING: label defined before entry/extern, the assembler will ignore that label"


#endif 
