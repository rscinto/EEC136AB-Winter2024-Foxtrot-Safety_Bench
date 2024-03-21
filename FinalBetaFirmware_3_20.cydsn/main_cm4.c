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


/*  HOW TO CALL DISPLAY
CLCD_Init() ;  //THIS GOES ABOVE LOOP IN MAIN
        CyDelay(500);
        

        go_to_top_line() ;
        CLCD_PutString("test1") ;
        go_to_bottom_line();
        CLCD_PutString("test2") ;

        CyDelay(500);
*/

//used with code from the gracious user and contributer: https://community.infineon.com/t5/Code-Examples/CharLCD-Sample-for-PSoC-6-CY8CKIT-062-BLE/td-p/268325
#include "project.h"
#include "stdio.h"
#include "clcd.h"
#include <string.h>
#include <math.h>

#define LIS3DH_ADDRESS            	0x19 //0x18 Black / 0x19 Red
#define SampleRate                  10 // # of sample taken per RMS readout  //WORKING:3
//#define rmsThreshold                10.5 // Threshold RMS value of all 3 axis
#define rmsThreshold                5 // not 0.5, 2, 10, 7WORKS with gusto: lvl 1


//Write Registers Defined
#define CTRL_REG1                   0x20
#define CTRL_REG3                   0x22
#define INT1_CFG                    0x30
#define CLICK_CFG                   0x38

//Read Register Defined
#define OUT_X_H      0x29        //High X 
#define OUT_X_L      0x28        //Low X 
#define OUT_Y_H      0x2B        //High Y
#define OUT_Y_L      0x2A        //Low Y 
#define OUT_Z_H      0x2D        //High Z 
#define OUT_Z_L      0x2C        //Low Z 

//Note(CR# = Control Register #) Edit Values below to change register
#define CR1_Settings                0x51 // 0x9F = 1001 1111 | 0x51 = 0101 0001  | 4 bits - Data Selection Rate / 1 Bit - Low Power Mode (ON/OFF) / 3 bits - 3 Axis Options [Z,Y,X](ON/OFF)
#define CR3_Settings                0xC0 // 0xC0 = 1100 0000
//#define INT1_CFG_Settings           0xE0 // 0xE0 = 1110 0000
//#define CLICK_CFG_Settings          0x2A // 0x2A = 0010 1010

//Interrupt Function
//void emergency_detected_handler();

void do_help(void) ;
void do_clear(void) ;
void do_write(void) ;
void do_SetDDRAddr(void) ;
void do_SetCGRAMAddr(void) ;
void do_Command(void) ;
void do_print(void) ;
void do_pos(void) ;
void do_reset(void) ;
void go_to_top_line(void);
void go_to_bottom_line(void);
void print_two_sentence(char str_one[16], char str_two[16]);

static void WriteRegister(uint8 reg_addr, uint16 data);
static void WaitForOperation(void);
static uint8 ReadRegister(uint8 reg_addr, int read_len);

static cy_stc_scb_i2c_master_xfer_config_t register_setting;
static uint8_t rbuff[6];
static uint8_t wbuff[6];

float x_accl = 0, y_accl = 0, z_accl = 0;
float x, y, z;
char str_accelerometer_value[16];
char top_line[16];
char bottom_line[16];
int state = 1;
int last_state = 0;
int light_switch=0;
int I2C_count = 0;

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */
    CLCD_Init() ;
    
    //Enabling interrupt and linking to function
   // Cy_SysInt_Init(&PWR_OFF_Interrupt_cfg, emergency_detected_handler);
   // NVIC_EnableIRQ(PWR_OFF_Interrupt_cfg.intrSrc);
    
    //turn all of the logical LEDs OFF at boot 
    Cy_GPIO_Write(fault_led_PORT, fault_led_NUM, 1);
    Cy_GPIO_Write(Inertial_emergency_LED_PORT, Inertial_emergency_LED_NUM, 1);
    Cy_GPIO_Write(Estop_emergency_LED_PORT, Estop_emergency_LED_NUM, 1);
    
    //Turn on LCD power 
    //Cy_GPIO_Write(LCD_POWER_PORT, LCD_POWER_NUM, 1);
    
    go_to_top_line() ;
    CLCD_PutString("Booting") ;
    go_to_bottom_line();
    CLCD_PutString("Stand By") ;
    CyDelay(1000);
    
    Cy_GPIO_Write(fault_led_PORT, fault_led_NUM, 0);
    Cy_GPIO_Write(Inertial_emergency_LED_PORT, Inertial_emergency_LED_NUM, 0);
    Cy_GPIO_Write(Estop_emergency_LED_PORT, Estop_emergency_LED_NUM, 0);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    //UART_Start(); //starting the UART
    
    setvbuf(stdin,NULL, _IONBF,0);
    
    
    
    
    
    SensorBus_Start(); //I2C
    
    NVIC_EnableIRQ((IRQn_Type) SensorBus_SCB_IRQ_cfg.intrSrc); 

    register_setting.slaveAddress =LIS3DH_ADDRESS;
    WriteRegister(CTRL_REG1, CR1_Settings); //going up to 400Hz
    //WriteRegister(CTRL_REG3, CR3_Settings); //Sets up interupt tried(C0) no change
    //WriteRegister(INT1_CFG, INT1_CFG_Settings); // //NO CHANGE
    //WriteRegister(CLICK_CFG, CLICK_CFG_Settings); //Sets up config <-sets up double tap //NO CHANGE (TRY 2A)

    WriteRegister(0x1F, 0b11000000); // temp_cfg_reg
    WriteRegister(0x23, 0b10000000);

    float x_accl = 0, y_accl = 0, z_accl = 0;
    float x, y, z;
    char g_force_fault[16];
    int i = 0;
    char c;
    
    
    CyDelay(50);
        
        
        
        
        
    

   

    for(;;)
    {
        
/*    
    
    Cy_SCB_I2C_Disable(SensorBus_HW, &SensorBus_context);
    Cy_SCB_I2C_Enable(SensorBus_HW);
        
            
    SensorBus_Start(); //I2C
    
    NVIC_EnableIRQ((IRQn_Type) SensorBus_SCB_IRQ_cfg.intrSrc); 

    register_setting.slaveAddress =LIS3DH_ADDRESS;
    WriteRegister(CTRL_REG1, CR1_Settings); //going up to 400Hz
    //WriteRegister(CTRL_REG3, CR3_Settings); //Sets up interupt tried(C0) no change
    //WriteRegister(INT1_CFG, INT1_CFG_Settings); // //NO CHANGE
    //WriteRegister(CLICK_CFG, CLICK_CFG_Settings); //Sets up config <-sets up double tap //NO CHANGE (TRY 2A)

    WriteRegister(0x1F, 0b11000000); // temp_cfg_reg
    WriteRegister(0x23, 0b10000000);
*/    
/*   
    CyDelay(50);

        x = (int16_t)(ReadRegister(OUT_X_L, 1) | (ReadRegister(OUT_X_H, 1) << 8));
        x_accl = ((2.0 * x) / 32767) * 9.8;
        
        print_two_sentence(top_line,bottom_line);
        
        if(x_accl < 5)
                {
                    state = 4;
                    
                if (last_state != state)
                {
                    sprintf(top_line, "%0.2f", x_accl);
                    sprintf(bottom_line, "FAULT");
                    print_two_sentence(top_line,bottom_line);
                }
                    last_state = state;
                }
        else
        {
            state = 1;
            if (last_state != state)
            {
                sprintf(top_line, "%0.2f", x_accl);
                sprintf(bottom_line, "Normal Operation");
                print_two_sentence(top_line,bottom_line);

            }
            last_state = state;

        }
*/                 

    if(Cy_GPIO_Read(sampleV_PORT,sampleV_NUM) == 0)
    {
        if(Cy_GPIO_Read(Estop_test_PORT,Estop_test_NUM) == 0 && (state == 1)) {
            state = 2;
            if (last_state != state)
            {
            sprintf(top_line, "ESTOP");
            sprintf(bottom_line, "EMERGENCY");
            print_two_sentence(top_line,bottom_line);
            }
            last_state = state;
        
        }
        else if(Cy_GPIO_Read(Estop_test_PORT,Estop_test_NUM) != 0 && (state == 1))
        {
            Cy_SCB_I2C_Disable(SensorBus_HW, &SensorBus_context);
            Cy_SCB_I2C_Enable(SensorBus_HW);
        
            
            SensorBus_Start(); //I2C
    
            NVIC_EnableIRQ((IRQn_Type) SensorBus_SCB_IRQ_cfg.intrSrc); 

            register_setting.slaveAddress =LIS3DH_ADDRESS;
            WriteRegister(CTRL_REG1, CR1_Settings); //going up to 400Hz
            //WriteRegister(CTRL_REG3, CR3_Settings); //Sets up interupt tried(C0) no change
            //WriteRegister(INT1_CFG, INT1_CFG_Settings); // //NO CHANGE
            //WriteRegister(CLICK_CFG, CLICK_CFG_Settings); //Sets up config <-sets up double tap //NO CHANGE (TRY 2A)

            WriteRegister(0x1F, 0b11000000); // temp_cfg_reg
            WriteRegister(0x23, 0b10000000);
            x = (int16_t)(ReadRegister(OUT_X_L, 1) | (ReadRegister(OUT_X_H, 1) << 8));
            x_accl = ((2.0 * x) / 32767) * 9.8;
            
            if(x_accl < 100)
            {
                state = 4;
                
            if (last_state != state)
            {
            sprintf(top_line, "INERTIAL FAULT");
            sprintf(bottom_line, "Accel Val:%.2f", x_accl);
            print_two_sentence(top_line,bottom_line);
            }
            last_state = state;
            }
            else
            {

            state = 3;
            if (last_state != state)
            {
            sprintf(top_line, "INERTIAL");
            sprintf(bottom_line, "EMERGENCY");
            print_two_sentence(top_line,bottom_line);
            }
            last_state = state;
        }
    }
    }
    else
    {
        state = 1;
        if (last_state != state)
        {
        sprintf(top_line, "Normal Operation");
        sprintf(bottom_line, "Stay Alert");
        print_two_sentence(top_line,bottom_line);
        }
        last_state = state;

    }
    
    if(state == 1)
    {
        Cy_GPIO_Write(fault_led_PORT,fault_led_NUM,0);
        Cy_GPIO_Write(Inertial_emergency_LED_PORT,Inertial_emergency_LED_NUM,0);
        Cy_GPIO_Write(Estop_emergency_LED_PORT,Estop_emergency_LED_NUM,0);
    }
    else if(state == 2)
    {
        Cy_GPIO_Write(fault_led_PORT,fault_led_NUM,0);
        Cy_GPIO_Write(Inertial_emergency_LED_PORT,Inertial_emergency_LED_NUM,0);
        
        Cy_GPIO_Write(Estop_emergency_LED_PORT,Estop_emergency_LED_NUM,light_switch);
    }
    else if(state == 3)
    {
        Cy_GPIO_Write(fault_led_PORT,fault_led_NUM,0);
        Cy_GPIO_Write(Inertial_emergency_LED_PORT,Inertial_emergency_LED_NUM,light_switch);
    
        Cy_GPIO_Write(Estop_emergency_LED_PORT,Estop_emergency_LED_NUM,0);
        
    }
    else if (state == 4)
    {
        Cy_GPIO_Write(fault_led_PORT,fault_led_NUM,1);
        
        Cy_GPIO_Write(Inertial_emergency_LED_PORT,Inertial_emergency_LED_NUM,light_switch);
        Cy_GPIO_Write(Estop_emergency_LED_PORT,Estop_emergency_LED_NUM,0);
    
    }
    
    if(light_switch == 1)
    {
        light_switch = 0;
    } else
    {
        light_switch = 1;
    }
}
}
void print_two_sentence(char str_one[16], char str_two[16])
{
    CLCD_Init();
    CyDelay(50);
    do_clear();
    go_to_top_line() ;
    CLCD_PutString(str_one) ;
    go_to_bottom_line();
    CLCD_PutString(str_two);
    CyDelay(50);
}










void go_to_top_line()
{
    int line = 1, col = 0;
    int addr ;
    //sscanf(str, "%*s %d %d", &line, &col) ;
    if (line < 1) { 
        line = 1 ;
    }
    if (line > CLCD_NUM_ROWS) {
        line = CLCD_NUM_ROWS ;
    }
    if (col < 1) {
        col = 1 ;
    } 
    if (col > CLCD_NUM_COLS) {
        col = CLCD_NUM_COLS ;
    }
    addr = 0x40 * (line-1) + (col-1) ;
    CLCD_SetDDRAMAddr(addr) ;
    
    
}





void go_to_bottom_line()
{
    int line = 2, col = 0;
    int addr ;
    //sscanf(str, "%*s %d %d", &line, &col) ;
    if (line < 1) { 
        line = 1 ;
    }
    if (line > CLCD_NUM_ROWS) {
        line = CLCD_NUM_ROWS ;
    }
    if (col < 1) {
        col = 1 ;
    } 
    if (col > CLCD_NUM_COLS) {
        col = CLCD_NUM_COLS ;
    }
    addr = 0x40 * (line-1) + (col-1) ;
    CLCD_SetDDRAMAddr(addr) ;
}



void do_clear(void) 
{
    CLCD_Clear() ;
}

void do_write(void) 
{
    int c ;
    //sscanf(str, "%*s %x", &c) ;
    CLCD_WriteData(c & 0xFF) ;
}

void do_SetDDRAddr(void) 
{
    int addr ;
    //sscanf(str, "%*s %x", &addr) ;
    CLCD_SetDDRAMAddr(addr) ;
}

void do_SetCGRAMAddr(void) 
{
    int addr ;
    //sscanf(str, "%*s %x", &addr) ;
    CLCD_SetCGRAMAddr(addr) ;    
}

void do_Command(void) 
{
    int cmd ;
    
    //sscanf(str, "%*s %x", &cmd) ;
    CLCD_WriteCommand(cmd) ;
}

void do_print(void)
{
    char buf[81] ;
    //sscanf(str, "%*s %s", buf) ;
    CLCD_PutString(buf) ;
}

void do_pos(void)
{
    int line, col ;
    int addr ;
    //sscanf(str, "%*s %d %d", &line, &col) ;
    if (line < 1) { 
        line = 1 ;
    }
    if (line > CLCD_NUM_ROWS) {
        line = CLCD_NUM_ROWS ;
    }
    if (col < 1) {
        col = 1 ;
    } 
    if (col > CLCD_NUM_COLS) {
        col = CLCD_NUM_COLS ;
    }
    addr = 0x40 * (line-1) + (col-1) ;
    CLCD_SetDDRAMAddr(addr) ;
}

void do_reset(void)
{
    CLCD_Init() ;
}







static void WaitForOperation()
{
    int busy_counter = 0;
    
 while(0 != (CY_SCB_I2C_MASTER_BUSY & SensorBus_MasterGetStatus())){}
    {
       /*
        busy_counter++;
        if (busy_counter == 10)
        {
            Cy_GPIO_Write(SDA_PORT, SDA_NUM, 1);
            
            for(int i=0; i<=9; i++)
            {
                Cy_GPIO_Write(SCL_PORT, SCL_NUM, 1);
                CyDelay(2);
                Cy_GPIO_Write(SCL_PORT, SCL_NUM, 0);
                CyDelay(2);
            }

            Cy_GPIO_Write(SDA_PORT, SDA_NUM, 0);
            CyDelay(5);
            Cy_GPIO_Write(SCL_PORT, SCL_NUM, 1);
            CyDelay(2);
            Cy_GPIO_Write(SDA_PORT, SDA_NUM, 1);
            CyDelay(2);
            busy_counter = 0;
        }
        */
        CyDelayUs(1);
    }



}





static void WriteRegister(uint8 reg_addr, uint16 data)
{
    wbuff[0] = reg_addr;
    wbuff[1] = data;
    
    register_setting.buffer = wbuff;
    register_setting.bufferSize = 2;
    register_setting.xferPending = false;
    
    SensorBus_MasterWrite(&register_setting);
    WaitForOperation(); 
}





static uint8 ReadRegister(uint8 reg_addr, int read_len)
{
    wbuff[0] = reg_addr;
    
    register_setting.buffer = wbuff;
    register_setting.bufferSize = 1;
    register_setting.xferPending = true;
           
    
    SensorBus_MasterWrite(&register_setting);
    WaitForOperation();
    
    register_setting.buffer = rbuff;
    register_setting.bufferSize = read_len;
    register_setting.xferPending = false;
    
    SensorBus_MasterRead(&register_setting);
    WaitForOperation();
    //CyDelay(50);
    
    return rbuff[0];   
}






/* [] END OF FILE */
