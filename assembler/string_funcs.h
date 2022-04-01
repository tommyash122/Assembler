#ifndef STRING_FUNCS_H
#define STRING_FUNCS_H

#include "Definitions.h"

char * AdvWhiteChars(char * );
boolean line_is_comment(char * );
boolean white_line(char );
void   AdvWhiteChars_fromEnd(char * );
char * AdvWhiteChars_both_sides(char * );
boolean signed_integer(char );
boolean valid_last_char(char );
boolean unexpected_comma(char) ;
boolean valid_length(char *);

#endif
