#include "Definitions.h"
#include "data_structures.h"
#include "first_run.h"
#include "second_run.h"
#include "output.h"
#include "macro_set.h"


int main(int argc ,char * argv[] )
{
	if(argc < VALID_NUMBER_OF_ARGS)
	{
		printf("No arguments received\n");
	}
	else  /*we have got at least on file to procces*/
	{
		int i; /* Counter to traverse the input arguments */
		
		
		for( i=1 ; i<argc ; i++ )
		{
			/*declarations of variables, as required by the ANCI-C*/
            int num_of_rows = 0; /* number of rows in the file */
            char * file_name = argv[i];

            int IC = INIT_MEMORY_ADDRESS;
            int DC = 0;
            

            line_Struct * whole_lines_table_for_file; 
			root * root_of_tables; /*A pointer to a struct root that holds pointers to the other tables*/
			
			
			if(!macro_set(file_name , &num_of_rows))
				continue;
       		 		
       		/* . . .tables for this specific file . . .*/	
       		/*allocated memory for the whole file, for each line*/
       		whole_lines_table_for_file = (line_Struct *)malloc((num_of_rows) * sizeof(line_Struct));
       		
       		/*A struct that will hold pointers to all the tables*/
       		root_of_tables = (root *)malloc(sizeof(root));
       		
       		
       		/*if we didn't find a place to allocate memory*/
       		if( (!whole_lines_table_for_file) || (!root_of_tables) ) 
       		{
       			printf("PROGRAM_ERROR: memory allocation failed");
       			deallocate_list(root_of_tables ,EXIT);
       		}
       		
     
       		root_initializer(root_of_tables , whole_lines_table_for_file);
       		
			printf("\n.....start first run.....\n");
			
			if( !first_run(file_name, whole_lines_table_for_file, root_of_tables , &IC , &DC , num_of_rows ) )     
			{
       			deallocate_list(root_of_tables , NO_EXIT);
       			printf("\n.....Continue to the next file.....\n\n\n");
      			
      		 continue;
			}
			
			printf("\n.....start second run.....\n");

			if( !second_run(file_name, whole_lines_table_for_file, root_of_tables , num_of_rows ) )     
			{
       			deallocate_list(root_of_tables , NO_EXIT);
       			printf("\n.....Continue to the next file.....\n\n\n");
      		
      		 continue;
			}
			
			printf("\n.....create output.....\n");
			
			
			create_output(file_name, root_of_tables );
       		
       		/*finished with this file, free all the allocated memory*/
       		deallocate_list(root_of_tables , NO_EXIT);
            printf("\n.....Continue to the next file.....\n\n\n");
		}
	}
 	printf("No more files to process, goodbye!\n");
 return 0;
}



