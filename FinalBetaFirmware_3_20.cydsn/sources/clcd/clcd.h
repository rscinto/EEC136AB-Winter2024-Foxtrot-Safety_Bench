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
#ifndef _CLCD_H_
#define _CLCD_H_
#include "project.h"
    
#define CLCD_NUM_ROWS 2
#define CLCD_NUM_COLS 16
    
void CLCD_Init(void) ;
void CLCD_Start(void) ;

void CLCD_WriteCommand4(uint8_t cmd) ;
void CLCD_WriteCommand(uint8_t cmd) ;
void CLCD_WriteData(uint8_t cmd) ;
int  CLCD_IsBusy(void) ;
void CLCD_Clear(void) ;
void CLCD_Home(void) ;
void CLCD_Display(uint8_t mode) ;
void CLCD_SetFunction(uint8_t mode) ;
void CLCD_SetCGRAMAddr(uint8_t addr) ;
void CLCD_SetDDRAMAddr(uint8_t addr) ;

void CLCD_PutChar(uint8_t c) ;
void CLCD_PutString(char str[]) ;
    
#endif /* _CLDC_H_ */
/* [] END OF FILE */
