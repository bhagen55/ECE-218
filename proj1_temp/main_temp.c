/* 
 * File:   main_temp.c
 * Author: Blair Hagen
 *
 * Created on January 11, 2017, 3:28 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Chip Setup
#include "p24hxxxx.h"						/* generic header for PIC24H family */
#include "stdint.h"	
#include "adc.h"                // Analog->Digital conversion headers

// Delay Setup
#define FCY 3685000L             //needed for X16 compiler library delays in libpic30.h
#include <libpic30.h>           //includes definitions for __delay_ms and __delay_us

#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

#include "lcd_SF.h"             // LCD header file

/*
 * ADC storage variables
 */
uint16_t rawADCVal = 0;
double voltVal = 0.0;
double celTemp = 0;
double farTemp = 0;
double cal = -0.0175; // used to calibrate voltage reading
double millivoltVal = 0;

/*
 * LCD storage variables
 */
char voltageStr[40];
char tempStr[40];
//int degreeSym = "223";

/*
 * temperature range variables
 */
int t2 = 27; // upper celcius range
int v2 = 887; // upper voltage range
int t1 = 16; // lower celcius range
int v1 = 947; // lower voltage range

/*
 * Convert raw value from ADC to a Farenheit float
 * Using accurate equation
 */
float convToCelAcc(float rawVal)
{
    
    float squareRoot = sqrt(0.00704 * (870.6 - rawVal) + 30.316);
    float tempVal = squareRoot * 284.091 + 35.506;
    
    return tempVal;
}

/*
 * Convert using approximation to 50f - 81f (10c - 26c) 
 */
float convToCelApprox(float rawVal)
{
    float tempVal = ((v1*t2) - (v2*t1) + rawVal * (t1 - t2))/(v1 - v2);
    
    return tempVal;
}

/*
 * Convert farenheit to celcius
 */
float convCelToFar(float celVal)
{
    float farVal = (celVal * 1.8) + 32;
    
    return farVal;
}


/*
 * Reads voltage from thermistor
 * calculates temperature value (in farenheit)
 * writes to LCD screen with units
 */
int main(void) {
    
    /*
     * ADC Setup
     */
    initADC(AN1); //Initialize pin AN1 to analog->digital converter
    
    /*
     * LCD Setup
     */
    Init_LCD();         // Initialize LCD Screen
    ClearScreen();      // Clear LCD Screen
    
    while (1) 	// Loop forever
	{
		rawADCVal = readADC (AN1);	// Read from ADC on pin AN1
        voltVal = ((rawADCVal * 3.3)/4096) + cal; // Convert from raw ADC value to real world voltage. Calibration value included.
        millivoltVal = voltVal * 1000; // Change to millivolts
        
        WriteCMD(0b10000000); // Cursor to beginning
        
        sprintf(voltageStr, "Voltage: %.2f", millivoltVal); // Print voltage
        
        WriteString_LCD(voltageStr);
        
        celTemp = convToCelApprox(millivoltVal);
        farTemp = convCelToFar(celTemp);
        
        sprintf(tempStr, "Temp: %.1f", farTemp); // Print temperature
        
        
        WriteCMD(0b11000000);
        WriteString_LCD(tempStr); // write temperature
        
        WriteCMD(0b11001010);
        WriteData(0b11011111); // write degrees symbol
        
        WriteCMD(0b11001011); 
        WriteString_LCD("f"); // write f
        
		__delay_ms(10);
	}	
}



