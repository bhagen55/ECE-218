/* 
 * File:   main_lcd.c
 * Author: Blair Hagen
 *
 * Created on January 11, 2017, 2:05 PM
 */

#include <stdio.h>
#include <stdlib.h>

// Chip Setup
#include "p24hxxxx.h"						/* generic header for PIC24H family */
#include "stdint.h"					
#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

#include "lcd_SF.h"             // LCD header file


int main(void) {
    
    /*
     1. Initialize LCD
     2. Clear LCD
     3. Send message loop
     */
    
    Init_LCD();         // Initialize LCD Screen
    ClearScreen();      // Clear LCD Screen
    
    int i = 0;
    
    while (1)
    {
        
        /*
         1. Position cursor at 00
         2. Write 'Hello there!'
         3. Position cursor at 40
         3. Write 'Enjoy LCD Demo'
         */
        
        WriteCMD(0b10000000);
        
        WriteString_LCD("Hello there!");
        
        WriteCMD(0b11000000);
        
        WriteString_LCD("Enjoy LCD Demo");
        
        WriteCMD(0b10001111);
        if (i == 0)
        {
            i = 1;
            WriteString_LCD("<");
        }
        
        else
        {
            i = 0;
            WriteString_LCD(">");
        }
    }
    
    
    
    
}

