/* 
 * File:   ser_main.c
 * Author: bhage
 *
 * Created on February 8, 2017, 2:07 PM
 */

// Includes
#include <stdint.h>
#include "ser.h"
#include "p24Hxxxx.h"

// Chip Configuration
#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

// UART conversion variables
char UARTraw;

// Initialize IO: RX/TX for serial on pins 10/11
void initIO(void) 
{
    __builtin_write_OSCCONL(OSCCON & 0xbf); //clear bit 6 -unlock PPS
    RPINR19bits.U2RXR = 10;                 // make pin RP10 U2RX I/O
    RPOR5bits.RP11R = 5;                    // make RP11 U2TX
    __builtin_write_OSCCONL(OSCCON | 0x40); //set bit 6 - lock PPS
}

// Set up UART interrupt and read from U2RXREG
// Stores raw 16 bit data as an 8 bit char
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void) 
{    
    UARTraw = (char) U2RXREG & 0x00FF;   
    _U2RXIF = 0; // Clear interrupt flag
}

/*
 * Initialize IO
 * Enable UART2
 * Enable interrupts
 * Print message to screen
 * Continuously read character then send to screen
 */
int main(void) 
{
    initIO();       // Initialize IO pins for RX/TX
    initUART2();
    
    clrscr();
    home();
    
    // Enable interrupt
    _U2RXIF = 0;
    
    
    //char hello[6] = {'h', 'e', 'l', 'l', 'o', '\0'};
    putsU2("hello");
    
    while(1)
    {
        putU2(getU2());
    }
}

