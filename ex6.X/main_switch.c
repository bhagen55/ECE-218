/* 
 * File:   main_switch.c
 * Author: student
 *
 * Created on January 25, 2017, 3:24 PM
 */

#include <stdint.h>
#include "p24Hxxxx.h"

#define FCY 3685000L             //needed for X16 compiler library delays in libpic30.h
#include <libpic30.h>           //includes definitions for __delay_ms and __delay_us

#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

#define p_min 345
#define p_max 1036
#define PWM_PERIOD 9216

#define led LATAbits.LATA0
#define switch PORTBbits.RB15

uint16_t pulse_width = p_min;


void initOC1 (void) {
    T2CONbits.TON = 0; // Turn off Timer2
    PR2 = PWM_PERIOD;
    OC1RS = p_min;
    OC1R = p_min;
    OC1CONbits.OCTSEL = 0b000; // Set Timer2 as clock source for OC1
    OC1CONbits.OCM = 0b110; // 
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TCS = 0;
    T2CONbits.TON = 1;
    
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    _T2IF = 0;
    OC1RS = pulse_width;
 
   // led = ~led;
       
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    _T1IF = 0;
    led = ~led;
 
   // led = ~led;
       
}

void configIO( void ) {
        __builtin_write_OSCCONL(OSCCON & 0xbf); // clear bit 6 unlock
        RPOR0bits.RP1R = 0b10010;
        __builtin_write_OSCCONL(OSCCON | 0x40);
    }



int main(int argc, char** argv) {
    configIO();
    initOC1();
    
    AD1PCFGLbits.PCFG0 = 0b1;
    AD1PCFGL = 0xFFFF;
    TRISAbits.TRISA0 = 0b0;
    TRISBbits.TRISB15 = 0b1;
    
    _T2IF = 0;
    _T2IE = 1;
    
    
    //TMR1 = 0;
    //T1CON = 0x8030;
    
    //PR1 = 14394;
    
    //_T1IF = 0;
    //_T1IE = 1;
    
    while (1) {
        
        if (switch == 1) {
            led = ~led;
            __delay_ms(500);
        }
        
        if (switch == 0) {
            int i;
            for (i=p_min; i<=p_max; i++) {
                pulse_width++;
                __delay_ms(10);
            }
            for (i=p_max; i>=p_min; i--) {
                pulse_width--;
                __delay_ms(10);
            }
        }
    }
}

