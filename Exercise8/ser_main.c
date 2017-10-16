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

#define enable LATBbits.LATB8

#define PWM_PERIOD 376
#define p_min 188
#define p_max 376

// Motor variables
int oneA;
int twoA;

// Input Capture variables
uint16_t ICraw;
uint16_t ICold;
uint16_t ICcalc;
double time;
char disptime;

// UART conversion variables
char UARTraw;

// Initialize IO: RX/TX for serial on pins 10/11
void initIO(void) 
{
    RPOR0bits.RP1R = 0b10010; //Map pin1 (RP1R) (RP1=pin5)to OC1(0b10010))
    RPOR0bits.RP0R = 0b10011; // PR0 = pin4 
 
    RPINR7bits.IC1R = 6; // Connect IC1 to RP6 (Pin 15)
 
    __builtin_write_OSCCONL(OSCCON & 0xbf); //clear bit 6 -unlock PPS
    RPINR19bits.U2RXR = 10;                 // make pin RP10 U2RX I/O
    RPOR5bits.RP11R = 5;                    // make RP11 U2TX
    __builtin_write_OSCCONL(OSCCON | 0x40); //set bit 6 - lock PPS
}


//This function sets up the output compares OC1 and OC2, and Timer2
void initOC12(void)
{
    
    T2CONbits.TON = 0; //Turns off timer2
    PR2 = PWM_PERIOD; //Sets PR2 to be the period constant
    
    // OC1 setup
    OC1RS = p_min; //Sets OC1 secondary register to the minium period constant
    OC1R = p_min; //Sets OC1 register to the minium period constant
    OC1CONbits.OCTSEL = 0; //Set Timer2 as the clock source for OC1
    OC1CONbits.OCM = 0b110; //Set OC1 to PWM mode with fault pin disabled
    
    // OC2 setup
    OC2RS = p_min;
    OC2R = p_min;
    OC2CONbits.OCTSEL = 0; //Set Timer2 as the clock source for OC2
    OC2CONbits.OCM = 0b110; //Set OC2 to PWM mode with fault pin disabled  
    
    T2CONbits.TCS = 0;
    T2CONbits.TON = 1; //Turns timer2 on
    T2CONbits.TCKPS = 0b00; // Scale timer2 to 1:1
    
    // IC1 setup
    T3CONbits.TON = 0; // Turn timer3 off
    IC1CONbits.ICTMR = 0; // Set timer selection to timer3
    IC1CONbits.ICI = 0b00; // Set number of captures per interrupt bit to interrupt on every event capture
    IC1CONbits.ICM = 0b100; // Set input capture mode to every 4th rising edge
    
    T3CONbits.TCS = 0; // Set timer3 to internal osc
    T3CONbits.TCKPS = 0b00; // Scale timer3 to 1:1
    PR3 = 0xFFFF; // Initialize PR3 so timer runs full range
    T3CONbits.TON = 1; // Turn on timer3  
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) 
{     
    OC1RS = oneA;
    OC2RS = twoA;
    _T2IF = 0; 
}

void __attribute__((interrupt, no_auto_psv)) _IC1Interrupt(void)
{
    ICold = ICraw; // Store old value
    ICraw = IC1BUF; // Read new value
    ICcalc = (ICraw - ICold) >> 2;
    _IC1IF = 0;
    
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
    initOC12();
    
    TRISB = 0x0000;
    _T2IE = 1;
    //_T3IE = 1;
    _IC1IE = 1;
    
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

