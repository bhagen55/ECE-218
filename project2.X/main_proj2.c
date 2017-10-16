/*
 * File:   main_proj2.c
 * Author: student
 *
 * Created on January 26, 2017, 12:09 PM
 */

#include <stdint.h>
#include "p24Hxxxx.h"
#include "adc.h" 

#define FCY 3685000L             //needed for X16 compiler library delays in libpic30.h
#include <libpic30.h>           //includes definitions for __delay_ms and __delay_us

#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer


// Switch
#define switch PORTBbits.RB15   // Defines port to read to check switch voltage


#define PWM_PERIOD 9216 // PWM period of both servos

// Positional Servo. Defines minimum and maximum clock cycles for proper operations. Same for continuous
#define p_p_min 345
#define p_p_max 1036

uint16_t p_pulse_width = p_p_min; // Global variable for positional servo pwm width

// Continuous servo
#define c_p_min 600
#define c_p_max 783

uint16_t c_pulse_width = c_p_min; // Global variable for continuous servo pwm width

// ADC and Mapping
uint16_t rawADCVal = 0;
uint16_t mappedVal;


void initOC1 (void) {
    T2CONbits.TON = 0; // Turn off Timer2
    PR2 = PWM_PERIOD; // Set Timer2 preset register to PWM period of servo
    
    OC1RS = p_p_min; // Set OC1 to positional servo pwm settings
    OC1R = p_p_min;
    
    OC1CONbits.OCTSEL = 0b000; // Set Timer2 as clock source for OC1
    OC1CONbits.OCM = 0b110;  
    
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TCS = 0;
    T2CONbits.TON = 1;
    
}

void initOC2 (void) {
    T3CONbits.TON = 0;
    PR3 = PWM_PERIOD; // Set Timer3 preset register to PWM period of servo
    
    OC2RS = c_p_min; // Set OC2 to continuous servo pwm settings
    OC2R = c_p_min;
    
    OC2CONbits.OCTSEL = 0b001; // Set Timer3 as clock source for OC2
    OC2CONbits.OCM = 0b110;
    
    T3CONbits.TCKPS = 0b01;
    T3CONbits.TCS = 0;
    T3CONbits.TON = 1;
}

// Positional servo interrupt
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    _T2IF = 0;
    OC1RS = p_pulse_width;    
}

// Continuous servo interrupt
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    _T3IF = 0;
    OC2RS = c_pulse_width;   
}

void configIO( void ) {
        __builtin_write_OSCCONL(OSCCON & 0xbf); // clear bit 6 unlock
        RPOR0bits.RP1R = 0b10010;   // Connect OSC1 to pin RP1
        RPOR1bits.RP2R = 0b10011;   // Connect OSC2 to pin RP2
        __builtin_write_OSCCONL(OSCCON | 0x40); // Lock OSCCON
    }

// Scaling function
uint16_t scale(uint16_t X, uint16_t p_min, uint16_t p_max) {
    uint16_t range = p_max-p_min;
    uint32_t mult = (uint32_t)X * (uint32_t)range;
    uint32_t baseVal = mult/4096;
    return baseVal+ p_min;
}

int main(void) {
    configIO();
    initOC1();
    initOC2();
            
    initADC(AN1); // Initialize analog to digital converter on pin 3
    
    AD1PCFGLbits.PCFG0 = 0b1; // Set needed pins to digital for the switch
    AD1PCFGLbits.PCFG9 = 1;
    TRISAbits.TRISA0 = 0b0;
    TRISBbits.TRISB15 = 0b1;
    
    _T2IF = 0; // Enable interrupt flags
    _T2IE = 1;
    
    _T3IF = 0;
    _T3IE = 1;
    
    while (1) {
        rawADCVal = readADC(AN1); // Read from the potentiometer
        __delay_ms(10); // Leave enough delay for the ADC to get a new value
        
        if (switch == 0) {
            // Use POT for positional control
            p_pulse_width = scale(rawADCVal, p_p_min, p_p_max);
        }
        else {
            // Use POT for speed/direction control
            c_pulse_width = scale(rawADCVal, c_p_min, c_p_max);
            
        }
    }
    
    
}
