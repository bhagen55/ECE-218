/* 
 * File:   mainDCmotor.c
 * Author: student
 *
 * Created on February 1, 2017, 2:21 PM
 */

// Includes
#include "p24hxxxx.h" 
#include "stdint.h" 
#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include "ser.h" // Serial library
#include "adc.h"

// Chip Configuration
#pragma config FNOSC=FRC 
#pragma config FCKSM=CSDCMD 
#pragma config OSCIOFNC=ON 
#pragma config POSCMD=NONE
#pragma config FWDTEN = OFF 

// Delay Setup
#define FCY 3685000L
#include <libpic30.h>

// Motor enable pin
#define enable LATBbits.LATB8

// Motor values
#define PWM_PERIOD 376
#define p_min 188
#define p_max 376

// Motor variables
int adcvalue = 0;
int oneA;
int twoA;


// Input Capture variables
uint16_t ICraw = 100;
uint16_t ICold;
float ICcalc;
float time;
char disptime[100];
float RPS;
uint16_t motorRange;
uint16_t scaleadc;

// Switch setup
#define switch PORTBbits.RB15
char switchStat[20];

void configIO(void)
{
 __builtin_write_OSCCONL(OSCCON & 0xbf) ; //clear bit 6 unlock
 RPOR0bits.RP1R = 0b10010; //Map pin1 (RP1R) (RP1=pin5)to OC1(0b10010))
 RPOR0bits.RP0R = 0b10011; // PR0 = pin4 
 
 RPINR7bits.IC1R = 6; // Connect IC1 to RP6 (Pin 15)
 
 RPINR19bits.U2RXR = 10;                 // make pin RP10 U2RX I/O
 RPOR5bits.RP11R = 5;                    // make RP11 U2TX
 __builtin_write_OSCCONL(OSCCON | 0x40) ; //set bit 6 lock
}



//This function sets up the output compares OC1 and OC2, and Timer2
void initOC12(void)
{
    
    T2CONbits.TON = 0; //Turns off timer2
    T3CONbits.TON = 0; // Turn timer3 off
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
    T2CONbits.TCKPS = 0b00; // Scale timer2 to 1:1
    
    // IC1 setup    
    IC1CONbits.ICTMR = 0; // Set timer selection to timer3
    IC1CONbits.ICI = 0b00; // Set number of captures per interrupt bit to interrupt on every event capture
    IC1CONbits.ICM = 0b100; // Set input capture mode to every 4th rising edge
    
    T3CONbits.TCS = 0; // Set timer3 to internal osc
    T3CONbits.TCKPS = 0b00; // Scale timer3 to 1:1
    PR3 = 0xFFFF; // Initialize PR3 so timer runs full range
    T3CONbits.TON = 1; // Turn on timer3  
    
    T2CONbits.TON = 1; //Turns timer2 on
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

// Scaling function
uint16_t scale(uint16_t X, uint16_t p_min2, uint16_t p_max2) 
{
    uint16_t range = p_max2-p_min2;
    uint32_t mult = (uint32_t)X * (uint32_t)range;
    uint32_t baseVal = mult/4096;
    return baseVal+ p_min2;
}

int main()
{
    configIO();
    initOC12();
    initUART2();
    
    // Enable motor output pin to output
    TRISBbits.TRISB8 = 0b0;
    
    // Enable and clear interrupts
    _T2IE = 1;
    _IC1IE = 1;
    _IC1IF = 0;
    _U2RXIF = 0;
   
    // Setup LCD
    clrscr();
    home();
    
    // Initialize ADC
    initADC(AN1); //Initialize ADC for AN0 pin 2 of PIC24HJ64GP502
    
    // Motor enable
    enable = 1 ;
    
    while (1)
    {
        // Read from potentiometer and scale
        adcvalue = readADC (AN1); //force a conversion
        motorRange = scale(adcvalue, p_min, p_max);
        
      
        
        if (switch == 0)
        {
            oneA = motorRange;
            twoA = 0;            
        }
        else
        {
            oneA = 0;
            twoA = motorRange;            
        }            
        
        // Calculate and display speed and period
        time = ICcalc * 2.7137e-7;
        RPS = 1 / (1185.36 * time);
        home();
        sprintf(disptime, "Period (s): %.4f   RPS: %.2f", time, RPS); // Print time        
        __delay_ms(500);
        putsU2(disptime);
        putU2('\n');
        if (switch == 0)
        {
            putsU2("Direction: Clockwise");
        }
        else
        {
            putsU2("Direction: Anti-Clockwise");
        }
    }            
}


