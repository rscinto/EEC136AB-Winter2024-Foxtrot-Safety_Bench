#ifndef _CMD_FUNC_H_
#define _CMD_FUNC_H_
#include "project.h"
    
#define CMD_LENGTH 16

typedef void (*func_ptr)(void) ;

typedef struct _f_list_st_ {
    char    *name ;
    func_ptr func ;
    char    *description ;
} f_list_type ;

func_ptr get_func(f_list_type *list, char *cmd) ;

void     show_help(f_list_type *list) ;
    
#endif /* _CMD_FUNC_H_ */
