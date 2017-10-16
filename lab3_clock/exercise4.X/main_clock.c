/* 
 * File:   main_clock.c
 * Author: Blair Hageb
 *
 * Created on January 18, 2017, 2:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "p24hxxxx.h"

#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer


#define led LATAbits.LATA0

void _ISR _T1Interrupt(void) {
    led = ~led;
    _T1IF = 0;
}


int main( void ) {
    AD1PCFGL = 0xffff;
    LATA = 0x0000;
    TRISA = 0x0000;
    
    TMR1 = 0;
    T1CON = 0x8030;
    
    PR1 = 14394;
    
    _T1IF = 0;
    _T1IE = 1;
    
    while (1);
    
   
}

