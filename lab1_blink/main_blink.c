/* 
 * File:   main.c
 * Author: CT - Adapted from Kibalo Chapter 4 example
 *
 * Created on August 17, 2016, 1:07 PM
 */

#include "p24hxxxx.h"			/* generic header for PIC24H family */
#include "stdint.h"             //To use the standard types from textbook
/*
 *First need to add some basic configuration instructions. Five basic configurations are necessary.
 */						
#pragma config FNOSC=FRC         // set oscillator mode for FRC ~ 8 Mhz	
#pragma config FCKSM=CSDCMD      //Both clock switching and fail-safe clock monitor disabled
#pragma config OSCIOFNC=ON     // use OSCIO pin for digital I/O (RA3)
#pragma config POSCMD=NONE      //Primary Oscillator disabled
#pragma config FWDTEN = OFF		// turn off watchdog timer

#define led LATAbits.LATA0  // LED on microstick is connected to RA0 (PORTA, bit 0)
#define led2 LATAbits.LATA1 // Attached led to pin 3, board port 1

#define iend 125	    //delay loop is iend times jend instruction cycles
#define jend 3000
//delay function
	void delay(void) { 
		uint16_t i,j;       // unsigned integer of 16 bits (type)
		for (i=0; i<iend; i++)      // Incrementing loop 
			for (j=0;j<jend; j++); 	}

int main ( void )  //main function
{ /* Initialize ports */
	AD1PCFGL = 0xffff; 	//set all pins that share analog and make them digital
	LATA =0x0000;		// set latch levels for PORTA to all zeros
	TRISA =0x0000; 		// set IO as outputs for PORTA
    
	while (1) {     // loops forever
		delay();	// waits
        led = ~led;     // inverts led status
        led2 = ~led2;
		}
}