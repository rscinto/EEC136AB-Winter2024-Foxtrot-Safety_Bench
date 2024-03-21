#include "project.h"
#include "stdio.h"
#include "tty_utils.h"

uint32_t sema_num = SEMA_NUM ;
    
#define TAB   '\t'
#define SPACE ' ' 
#define CR    '\r'
#define LF    '\n'
#define BS    '\b'

#define RX_BUF_LEN  128

volatile char rx_buf[RX_BUF_LEN] ;
volatile int  rx_write_index = 0 ;
int           rx_read_index  = 0 ;
int           str_buf_index  = 0 ;
char          str[STR_BUF_LEN + 1] ;

static inline int is_delimiter(char c)
{
    return((c == TAB)||(c == SPACE)||(c == CR)||(c == LF)) ;
}

static inline int is_eol(char c)
{
    return((c == CR) || (c == LF)) ;
}

static inline int is_backspace(char c)
{
    return(c == BS) ;
}

void print(char *str)
{
    UART_PutString(str) ; /* for PSoC 4 */
}

void printc(char c) 
{
    UART_Put(c) ;
}

void tty_rx_isr(void)
{
    uint8_t c ;
    /* Check for "RX fifo not empty interrupt" */
    if((UART_HW->INTR_RX_MASKED & SCB_INTR_RX_MASKED_NOT_EMPTY_Msk ) != 0)
	{
        /* Clear UART "RX fifo not empty interrupt" */
		UART_HW->INTR_RX = UART_HW->INTR_RX & SCB_INTR_RX_NOT_EMPTY_Msk;    
        c =  Cy_SCB_UART_Get(UART_HW);
        if ((c != 0) && (c != 0xFF)) {
            rx_buf[rx_write_index] = c ;
            rx_write_index = (rx_write_index + 1) % RX_BUF_LEN ;
        }
    }
}

void cls(void)
{
    print("\033c") ; /* reset */
    CyDelay(20) ;
    print("\033[2J") ; /* clear screen */
    CyDelay(20) ;
}

void splash(char *prog_name) 
{
    cls() ;
    if (prog_name && *prog_name) {
        print(prog_name) ;
    } 
    print(" (") ;
    print(__DATE__) ;
    print(" ") ;
    print(__TIME__) ;
    print(")\n") ;
}

void prompt(char *str)
{
#if 0
    switch(current_core) {
    case USE_CM0P:
        print("CM0P:") ;
        break ;
    case USE_CM4:
        print("CM4:") ;
        break ;
    default: /* unknown core? forget it */
        break ;
    }
#endif
    if (str && *str) {
        print(str) ;
    } else {
        print("> ") ;
    }
}





int get_string(void)
{
    int result = 0 ;
    uint8_t c ;
    
    if (rx_read_index != rx_write_index) {
        c = rx_buf[rx_read_index] ;
        if (is_delimiter(c)) {
            printc(c) ;
            str[str_buf_index] = (char)NULL ;
            result = str_buf_index ;
            str_buf_index = 0 ;
        } else if (is_backspace(c)) {
            if (str_buf_index > 0) {
                str_buf_index-- ;
                str[str_buf_index] = 0 ;
                print("\b \b") ; // BS, SPACE, BS               
            }
        } else {
            printc(c) ;
            str[str_buf_index++] = c ;
            if (str_buf_index >= STR_BUF_LEN) {
                str[STR_BUF_LEN] = (char)NULL ;
                str_buf_index = 0 ;
                result = -1 ; /* str buf overflow */
            }
        }
        rx_read_index = (rx_read_index + 1) % RX_BUF_LEN ;
    }
    return( result ) ;
}

int get_line(void)
{
    int result = 0 ;
    uint8_t c ;
    
    if (rx_read_index != rx_write_index) {
        c = rx_buf[rx_read_index] ;
        if (is_eol(c)) {
            printc(c) ;
            str[str_buf_index] = (char)NULL ;
            result = str_buf_index ;
            str_buf_index = 0 ;
        } else if (is_backspace(c)) {
            if (str_buf_index > 0) {
                str_buf_index-- ;
                str[str_buf_index] = 0 ;
                print("\b \b") ;
            }
        } else {
            printc(c) ;
            str[str_buf_index++] = c ;
            if (str_buf_index >= STR_BUF_LEN) {
                str[STR_BUF_LEN] = (char)NULL ;
                str_buf_index = 0 ;
                result = -1 ; /* str buf overflow */
            }
        }
        rx_read_index = (rx_read_index + 1) % RX_BUF_LEN ;
    }
    return( result ) ;
}

uint8_t get_char(void)
{
    uint8_t c = 0 ;
    
    if (rx_read_index != rx_write_index) {
        c = rx_buf[rx_read_index] ;
        rx_read_index = (rx_read_index + 1) % RX_BUF_LEN ;
    }
    return( c ) ;   
}

int get_size_of_rx(void)
{
    int size = 0 ;
    if (rx_read_index != rx_write_index) {
        size = ((rx_write_index + RX_BUF_LEN) - rx_read_index) % RX_BUF_LEN ;
    }
    return( size ) ;
}

