/* LCD Display functions for Sparkfun LCD - CT*/
#include "p24hxxxx.h"						/* generic header for PIC24H family */
#define FCY 3685000L             //needed for X16 compiler library delays in libpic30.h
#include <libpic30.h>           //includes definitions for __delay_ms and __delay_us
#include "lcd_SF.h"
#include "stdint.h"

/* Functions to support LCD Display interface - Assumes 2-line LCD display with
 ST7066 controller in 4-bit mode and uses software delays in delay.c lib*/

void Init_LCD(void) {
    __delay_ms(50); //Wait for LCD to power up and settle
    //Set pins as outputs
    EN_DIRECTION = 0;
    RS_DIR = 0;
    RW_DIR = 0;
    LCD_BUS_DIR = 0;
    // Write zeros to  control pins and ports
    ENABLE = 0;  
    RS = 0;           //Control words will be written
    RW = 0;           //Writing to LCD, not reading
    __delay_ms(15);  //15ms delay per datasheet specs
    //Three writes to define 8-bit LCD 
    DB7 = 0;
    DB6 = 0;
    DB5 = 1;
    DB4 = 1;
    Pulse_Enable();
    __delay_ms(5);
    DB7 = 0;
    DB6 = 0;
    DB5 = 1;
    DB4 = 1;
    Pulse_Enable();
    __delay_us(100);
    DB7 = 0;
    DB6 = 0;
    DB5 = 1;
    DB4 = 1;
    Pulse_Enable();
    //commands to initialize LCD last two reversed from datasheet?
    //first command (0x20)is only 4-bits - to set interface as 4-bits
    DB7 = 0;
    DB6 = 0;
    DB5 = 1;
    DB4 = 0;
    Pulse_Enable();
    WriteCMD(0x28); //Function set: 2 lines, 5x8 font
    WriteCMD(0x28); //Function set: 2 lines, 5x8 font   
    WriteCMD(0x0C); //Display on, cursor on, no blink
    WriteCMD(0x01); //Clear Display
    WriteCMD(0x06); //Entry mode set: shift right
    __delay_ms(5);
}

void ClearScreen(void) {
    WriteCMD(0x01);
    __delay_ms(5);
}

void Pulse_Enable(void){
    ENABLE = 1;
    __delay_us(200);
    ENABLE = 0;
    __delay_us(200);
}

void WriteCMD(unsigned char Cmd){  
    uint8_t u8_upper;
     //Since we are not checking busy flag, delay 10ms
    __delay_ms(10);
    RS = 0;  //RS = 0 to write commands
    //Send 4 upper, then 4 lower bits
    //And pulse after each nibble
    u8_upper = Cmd >>4;
    DB7 = (u8_upper >> 3) & 0x01;
    DB6 = (u8_upper >> 2) & 0x01; 
    DB5 = (u8_upper >> 1) & 0x01;
    DB4 = (u8_upper & 0x01);
    Pulse_Enable();
    DB7 = (Cmd >> 3) & 0x01;
    DB6 = (Cmd >> 2) & 0x01; 
    DB5 = (Cmd >> 1) & 0x01;
    DB4 = (Cmd & 0x01);
    Pulse_Enable();  
}

void WriteData(unsigned char Data){  
    uint8_t u8_upper;
    //Since we are not checking busy flag, delay 10ms
    __delay_ms(10);
    RS = 1;  //RS = 1 to write data
    //Send 4 upper, then 4 lower bits
    //And pulse after each nibble
    u8_upper = Data >>4;
    DB7 = (u8_upper >> 3) & 0x01;
    DB6 = (u8_upper >> 2) & 0x01; 
    DB5 = (u8_upper >> 1) & 0x01;
    DB4 = (u8_upper & 0x01);
    Pulse_Enable();
    DB7 = (Data >> 3) & 0x01;
    DB6 = (Data >> 2) & 0x01; 
    DB5 = (Data >> 1) & 0x01;
    DB4 = (Data & 0x01);
    Pulse_Enable();   
}

void WriteString_LCD(const char *String)  
{
 while (*String != '\0')
 {
	WriteData(*String);
	String++;
}	
}