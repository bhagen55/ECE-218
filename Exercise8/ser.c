/*
 * File:   ser.c
 * Author: student
 *
 * Created on February 8, 2017, 2:04 PM
 */

#include "p24hxxxx.h"
#include "xc.h"
#include "ser.h"


#define FCY 3685000L //define the instruction clock used in BRG calculation
#define BAUDRATE 9600 //a standard baud rate available to most serial terminals
#define BRGVAL ((FCY/BAUDRATE)/16)- 1 //value for Baud Rate Generator register

void initUART2 (void)
{
  U2BRG = BRGVAL;
  U2MODEbits.UARTEN = 1;  
  U2STAbits.UTXEN = 1;  
}

void putU2 (char c)
{
   while (U2STAbits.UTXBF == 1);   
   U2TXREG = c; //Write the character to the U2TXREG
}

char getU2 (void)
{
    while (!U2STAbits.URXDA);
    return U2RXREG;
}

void putsU2 (char *s)
{
    while (*s != '\0')
    {
        putU2(*s);        
        s++;
    }  
    putU2('\r');
}

