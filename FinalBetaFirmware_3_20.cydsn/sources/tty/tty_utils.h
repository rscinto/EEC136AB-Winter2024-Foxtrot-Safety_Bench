#ifndef _TTY_UTILS_H_
#define _TTY_UTILS_H_
#include "project.h"
    
/* semaphore number to be used to switch cores */
#define SEMA_NUM 0u
#define USE_CM0P 1
#define USE_CM4  2

extern int current_core  ;
extern uint32_t sema_num ;
    
#define STR_BUF_LEN  64
 
void tty_init(void) ;

void tty_disable(void) ;

void print(char *str) ;

void printc(char c) ;

void cls(void) ;

/**
 * get_string()
 * check if we received any char via tty
 * and store the char into str[]
 * when a delimiter is detected
 * it puts NULL at the current end of str[] and return the length of str
 * if the length is exceeding that STR_BUF_LEN,
 * it puts NULL at the end of str[] and returns -1 
 */
int get_string(void) ;

/**
 * get_line() 
 * similar with get_string() but only accept CR or LF for delimiter
 * so you can get a line up to the STR_BUF_LEN 
 */
int get_line(void) ;

/**
 * get_char()
 * Returns the first byte in the RX_BUF or 0 if no data available
 */
uint8_t get_char(void) ;

/**
 * get_size_of_rx()
 * Returns bytes in the RX_BUF
 */
int get_size_of_rx(void) ;

void splash(char *prog_name) ;

void prompt(char *str) ;

extern char str[] ; /* print buf */

#endif /* _TTY_UTILS_H_ */
