#include "project.h"
#include "stdio.h"
#include "string.h"
#include "tty_utils.h"
#include "func_list.h"

func_ptr get_func(f_list_type *list, char *str)
{
    int      i ;
    func_ptr ptr = 0 ;
    char cmd[CMD_LENGTH+1] ;
    
    sscanf(str, "%s", cmd) ;
    for (i = 0 ; list[i].name ; i++ ) {
        if (strcmp(list[i].name, cmd) == 0) {
            break ;
        }
    }
    ptr = list[i].func ;
    
    return( ptr ) ;
}

void show_help(f_list_type *list)
{
    int i ;
    
    print("=== command ===\n\r") ;
    for (i = 0 ; list[i].name ; i++ ) {
        print(list[i].name) ;
        print(" : ") ;
        print(list[i].description) ;
        print("\n\r") ;
    }
    print("===============\n\r") ;  
    CyDelay(100) ;
}