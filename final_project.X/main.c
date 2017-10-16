/* 
 * File:   main.c
 * Author: Blair Hagen
 *
 * Created on March 2, 2017, 11:49 AM
 */

#include "stdint.h"
#include "stdio.h"
#include "p24Hxxxx.h"

#include "adc.h"
#include "lcd_SF.h"

#define FCY 3685000L             //needed for X16 compiler library delays in libpic30.h
#include <libpic30.h>           //includes definitions for __delay_ms and __delay_us

#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer


// Define pins
#define lightSensor AN1



// ADC storage variables
uint16_t rawLightVal = 0;
float millivoltVal = 0;

// LCD storage variables
char lightVoltageStr[40];

/*
 * Read from light sensor and convert to millivolts
 */
float readLight(void)
{
    rawLightVal = readADC(lightSensor);	// Read from ADC on pin AN1
    millivoltVal = ((rawLightVal * 3.3)/4096) * 1000; // Change to millivolts
    
    return millivoltVal;
}



int main(void) {
    /*
     * ADC Setup
     */
    initADC(lightSensor); //Initialize pin AN1 to analog->digital converter

    
    /*
     * LCD setup
     */
    Init_LCD();
    ClearScreen();
    
    /*
     * Main Loop
     */
    while (1)
    {
      // Send Light voltage to screen
      WriteCMD(0b10000000); // move cursor to start
      sprintf(lightVoltageStr, "Light Voltage: %.2f", readLight()); // Print voltage
      
    }
}

