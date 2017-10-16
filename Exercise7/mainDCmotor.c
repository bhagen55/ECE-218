/* 
 * File:   mainDCmotor.c
 * Author: student
 *
 * Created on February 1, 2017, 2:21 PM
 */

#include "p24hxxxx.h" 
#include "stdint.h" 
#include "xc.h"
#include <stdio.h>
#include <stdlib.h>

#pragma config FNOSC=FRC 
#pragma config FCKSM=CSDCMD 
#pragma config OSCIOFNC=ON 
#pragma config POSCMD=NONE
#pragma config FWDTEN = OFF 


#define FCY 3685000L
#include <libpic30.h>

#define enable LATBbits.LATB8


#define PWM_PERIOD 376
#define p_min 188
#define p_max 376

int oneA;
int twoA;

void configIO(void)
{
 __builtin_write_OSCCONL(OSCCON & 0xbf) ; //clear bit 6 unlock
 RPOR0bits.RP1R = 0b10010; //Map pin1 (RP1R) (RP1=pin5)to OC1(0b10010))
 RPOR0bits.RP0R = 0b10011; // PR0 = pin4 
 __builtin_write_OSCCONL(OSCCON | 0x40) ; //set bit 6 lock
}

//This function sets up the output compares OC1 and OC2, and Timer2
void initOC1(void)
{
    T2CONbits.TON = 0; //Turns off timer2
    PR2 = PWM_PERIOD; //Sets PR2 to be the period constant
    OC1RS = p_min; //Sets OC1 secondary register to the minium period constant
    OC1R = p_min; //Sets OC1 register to the minium period constant
    OC1CONbits.OCTSEL = 0; //Set Timer2 as the clock source for OC1
    OC1CONbits.OCM = 0b110; //Set OC1 to PWM mode with fault pin disabled
    OC2RS = p_min;
    OC2R = p_min;
    OC2CONbits.OCTSEL = 0; //Set Timer2 as the clock source for OC1
    OC2CONbits.OCM = 0b110; //Set OC1 to PWM mode with fault pin disabled      
    T2CONbits.TCS = 0;
    T2CONbits.TON = 1; //Turns timer2 on
    T2CONbits.TCKPS = 0b00;
    T3CONbits.TCKPS = 0b00;    
}


void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) 
{     
    OC1RS = oneA;
    OC2RS = twoA;
    _T2IF = 0; 
}

int main()
{
    TRISB = 0x0000;
    _T2IE = 1;
    _T3IE = 1;
    configIO();
    initOC1();

    while(1)
    {
        for (oneA = p_min; oneA <= p_max; oneA += 10) 
        {
            enable = 1;           
            twoA = 0;
            __delay_ms(60);
        }
        
        __delay_ms(1000);
        
        for (oneA = p_max; oneA >= p_min; oneA -= 10) 
        {
            enable = 1;
            twoA = 0;
            __delay_ms(60);
        }
        
        __delay_ms(1000);
        
        enable = 0;
        __delay_ms(1000);
        
        for (twoA = p_min; twoA <= p_max; twoA += 10) 
        {
            enable = 1;
            oneA = 0;
            __delay_ms(60);
        }
        
        __delay_ms(1000);
        
        for (twoA = p_max; twoA >= p_min; twoA -= 10) 
        {
            enable = 1;
            oneA = 0;
            __delay_ms(60);
        }
        
        __delay_ms(1000);
        
        enable = 0;
        __delay_ms(1000);        
        
    }      
}

