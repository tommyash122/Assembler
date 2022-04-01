#include "Definitions.h"
#include "data_structures.h"
#include "string_funcs.h"


/*skips the white chars*/
char * AdvWhiteChars(char * runner)
{
	while( (isspace(*runner)) && ((*runner)!='\n') && ((*runner) != '\0') )
		runner++;
	return runner;
}

/*blocking the string from the end, marking the end at each point until we face a non-white char*/
void AdvWhiteChars_fromEnd(char * runner)
{
	int end = ( (strlen(runner)) - 1 );
	
	for( ; end>=0 ; end--)
	{
		if( (isspace(runner[end])) || ((runner[end]) == '\0') )
			runner[end] = '\0';
		else
			break;
	}
}

char * AdvWhiteChars_both_sides(char * runner)
{
	AdvWhiteChars_fromEnd(runner);
	return AdvWhiteChars(runner);
}

boolean line_is_comment(char * runner)
{
	if((*runner) == ';')
		return TRUE;
	return FALSE;
}

boolean white_line(char ch)
{
	if ((ch == '\n') || (ch == EOF) || (ch == '\0') )
		return TRUE;
	return FALSE;
}

boolean unexpected_comma(char ch)
{
	if(ch == ',')
		return TRUE;
	return FALSE;
}

boolean signed_integer(char ch)
{
	if((ch == '+') || (ch == '-'))
		return TRUE;
	return FALSE;
}
boolean valid_last_char(char ch)
{
	if((isalnum(ch)) || (ch == ']') || (ch == '"'))
		return TRUE;
	return FALSE;
}

boolean valid_length(char * runner)
{
	if(strlen(runner) > MAX_LINE_LENGTH)
		return FALSE;
	return TRUE;
}

