/* 
 * File:   newmain.c
 * Author: CT - Adapted from Kibalo Chapter 4 example
 *
 * Created on August 17, 2016, 1:07 PM
 */
#include "p24hxxxx.h"						/* generic header for PIC24H family */
#include "adc.h"
#include "stdint.h"
/*
 *First need to add some basic configuration instructions. Five basic configurations are necessary.
 */						
#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

#define iend 25	//delay loop is iend times jend instruction cycles
#define jend 1000
uint16_t adcvalue = 0;
uint8_t count[] = {'0','0','0','0', '0'};
//delay function
void delay(void) { // 100 msec
	uint16_t i,j;
	for (i=0; i<iend; i++)
		for (j=0;j<jend; j++); 	}
    
/* ADC example- need potentiometer between +3.3V and Ground with wiper tied to pin 2 of PIC24H 	 */
int main(void)
{ 
	initADC(AN1); //Initialize ADC for AN0 pin 2 of PIC24HJ64GP502
	while (1) 	/*  endless loop: vary pot and capture reading*/
	{
		adcvalue =readADC (AN1);	//force a conversion
		binary_to_ASCIIconvert(adcvalue); // convert binary to ASCII
		count[0]= bcd10000; //store ASCII result in array
		count[1] =bcd1000;
		count[2] =bcd100;
		count[3] =bcdtens;
		count[4]= bcdunits;
		delay();	  
	}	
  }