/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "tty_utils.h"
#include "clcd.h"

#define LCD_V33 1
#define LCD_V50 0
#define MODE_8BIT 0
#define MODE_4BIT 1
#define DB_BIT7 0x80
#define DB_BIT6 0x40
#define DB_BIT5 0x20
#define DB_BIT4 0x10
#define DB_BIT3 0x08
#define DB_BIT2 0x04
#define DB_BIT1 0x02
#define DB_BIT0 0x01

#define DB7_Write(x)  Cy_GPIO_Write(CLCD_DB7_PORT, CLCD_DB7_NUM, x) 
#define DB6_Write(x)  Cy_GPIO_Write(CLCD_DB6_PORT, CLCD_DB6_NUM, x) 
#define DB5_Write(x)  Cy_GPIO_Write(CLCD_DB5_PORT, CLCD_DB5_NUM, x) 
#define DB4_Write(x)  Cy_GPIO_Write(CLCD_DB4_PORT, CLCD_DB4_NUM, x) 
#define DB3_Write(x)  Cy_GPIO_Write(CLCD_DB3_PORT, CLCD_DB3_NUM, x) 
#define DB2_Write(x)  Cy_GPIO_Write(CLCD_DB2_PORT, CLCD_DB2_NUM, x) 
#define DB1_Write(x)  Cy_GPIO_Write(CLCD_DB1_PORT, CLCD_DB1_NUM, x) 
#define DB0_Write(x)  Cy_GPIO_Write(CLCD_DB0_PORT, CLCD_DB0_NUM, x) 
#define RS_Write(x)   Cy_GPIO_Write(CLCD_RS_PORT,  CLCD_RS_NUM,  x)
#define RW_Write(x)   Cy_GPIO_Write(CLCD_RW_PORT,  CLCD_RW_NUM,  x) 
#define EN_Write(x)   Cy_GPIO_Write(CLCD_E_PORT,   CLCD_E_NUM,   x) 

#if LCD_V33 /* 3.3V operation */
#define CLCD_INITIAL_WAIT 40
 
#else /* 5V operation */
#define CLCD_INITIAL_WAIT 15
#endif

void CLCD_Init(void) 
{
    CyDelay(CLCD_INITIAL_WAIT) ;
    
    CLCD_WriteCommand4(0x30) ; /* Function mode 8bit mode */
    CyDelay(5) ; /* >= 4.1ms */
    CLCD_WriteCommand4(0x30) ; /* Function mode 8bit mode */
    CyDelayUs(100) ; /* >= 100us */
    CLCD_WriteCommand4(0x30) ; /* Funciton mode 8bit mode */
    
    CLCD_WriteCommand4(0x20) ; /* Function mode 4bit mode */
    
    CLCD_WriteCommand(0x2C) ; /* 2 lines, 11 pixel/line */
    
    CLCD_WriteCommand(0x0F) ; /* Char Display ON, cursor ON, Blink */
    
    CLCD_WriteCommand(0x01) ; /* Display Clear */
    
    CLCD_WriteCommand(0x02) ; /* Cursor Home */
    
    CLCD_WriteCommand(0x06) ; /* increment, no shift */
    
    CLCD_SetDDRAMAddr(0x00) ;
}

void CLCD_Start(void) ;

void CLCD_Write4(uint8_t data)
{
//    snprintf(str, STR_BUF_LEN, "%X\n", (data >> 4) & 0x0F) ;
//    print(str) ;
    DB4_Write((data >> 4) & 0x01) ;
    DB5_Write((data >> 5) & 0x01) ; 
    DB6_Write((data >> 6) & 0x01) ;    
    DB7_Write((data >> 7) & 0x01) ;
}

void CLCD_WriteCommand4(uint8_t cmd) 
{
//    snprintf(str, STR_BUF_LEN, "command4: %02X\n", cmd) ;
//   print(str) ;
    CyDelay(1) ;
    RS_Write(0) ; /* command */
    CyDelay(1) ;
    RW_Write(0) ; /* Write */
    
    CyDelayUs(100) ;

    EN_Write(1) ;
    CLCD_Write4(cmd) ;
      
    CyDelayUs(450) ;
    EN_Write(0) ;
    DB7_Write(0) ; // 14-Oct-2021   
    CyDelayUs(20) ;
    
    CyDelay(100) ;
}

void CLCD_WriteCommand(uint8_t cmd) 
{
//    snprintf(str, STR_BUF_LEN, "command8: %02X\n", cmd) ;
//    print(str) ;
    RS_Write(0) ; /* command */
    RW_Write(0) ; /* Write */
    
    CyDelayUs(100) ;

    EN_Write(1) ;
    CLCD_Write4(cmd) ;
      
    CyDelayUs(450) ;
    EN_Write(0) ;
    DB7_Write(0) ; // 14-Oct-2021    
    CyDelayUs(20) ;

    
    CyDelayUs(800) ;
    
    EN_Write(1) ;    
    CLCD_Write4((cmd & 0x0F) << 4) ;
    
    CyDelayUs(450) ;
    EN_Write(0) ;
    DB7_Write(0) ; // 14-Oct-2021        
    
    CyDelay(100) ; // 14-Oct-2021
}

void CLCD_WriteData(uint8_t data) 
{
//    snprintf(str, STR_BUF_LEN, "data: %02X\n", data) ;
//    print(str) ;
    CyDelay(1) ;
    RS_Write(1) ; /* data */
    CyDelay(1) ;
    RW_Write(0) ; /* write */
    
    CyDelayUs(100) ;
    
    EN_Write(1) ;
    CLCD_Write4(data) ;
    
    CyDelayUs(450) ;
    EN_Write(0) ;  
    DB7_Write(0) ; // 14-Oct-2021    
    
    CyDelayUs(800) ;
    
    EN_Write(1) ;
    CLCD_Write4((data & 0x0F) << 4) ;
    
    CyDelayUs(450) ;
    EN_Write(0) ;
    DB7_Write(0) ; // 14-Oct-2021  
    CyDelayUs(20) ;
    
    CyDelay(1) ;
}

int  CLCD_IsBusy(void) 
{
    int result = 0 ;
    /* read DB7... can we ? */
    return( result ) ;
}

void CLCD_Clear(void)
{
    CLCD_WriteCommand(0x01) ;
}

void CLCD_Home(void) 
{
    CLCD_WriteCommand(0x03) ;
}

void CLCD_Display(uint8_t mode) 
{
    CLCD_WriteCommand( 0x08 | (mode & 0x07)) ;
}

void CLCD_SetFunction(uint8_t mode) 
{
    CLCD_WriteCommand(0x20 | (mode & 0x1C)) ;
}

void CLCD_SetCGRAMAddr(uint8_t addr) 
{
    CLCD_WriteCommand(0x40 | (addr & 0x3F)) ;
}

void CLCD_SetDDRAMAddr(uint8_t addr) 
{
    CLCD_WriteCommand(0x80 | (addr & 0x7F)) ;
}

void CLCD_PutChar(uint8_t c) 
{
    CLCD_WriteData(c) ;
}

void CLCD_PutString(char str_buf[]) 
{
    int i = 0 ;
    
    while(str_buf[i]) {
        CLCD_WriteData(str_buf[i] & 0xFF) ;
        i++ ;
        CyDelay(10) ;
    }
    CyDelay(10) ;
}

/* [] END OF FILE */
