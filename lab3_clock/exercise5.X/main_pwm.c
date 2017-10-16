/* 
 * File:   main_pwm.c
 * Author: bhage
 *
 * Created on January 18, 2017, 3:29 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24Hxxxx.h"

#define FCY 3685000L
#include <libpic30.h>


#pragma config FNOSC=FRC         // set oscillator mode for FRC
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

#define p_min 9216 //345
#define p_max 1 //1036
#define PWM_PERIOD 9216

int pulse_width = 0;


void initOC1 (void) {
    T2CONbits.TON = 0;
    PR2 = PWM_PERIOD;
    OC1RS = p_min;
    OC1R = p_min; //?
    OC1CONbits.OCTSEL = 0; // Set Timer2 as clock source for OC1
    OC1CONbits.OCM = 0b110; //
    T2CONbits.TCKPS = 1;
    T2CONbits.TCS = 0; //?
    T2CONbits.TON = 1;
    
}

void _ISR_T2Interrupt(void) {
    OC1RS = pulse_width;
    _T2IF = 0;
}

void configIO( void ) {
        __builtin_write_OSCCONL(OSCCON & 0xbf); // clear bit 6 unlock
        RPOR0bits.RP1R = 0b10010;
        __builtin_write_OSCCONL(OSCCON | 0x40);
    }

int main( void ) {
    configIO();
    initOC1();
    
    _T2IF = 0;
    _T2IE = 1;
    
    while (1) {
        int i,j;
        for (i=0; i<9216; i++) {
            for (j=0; j<9216; j++) {
                pulse_width++;
                __delay_ms(50);
            }
            pulse_width--;
            __delay_ms(50);
            
        }
    }
}

