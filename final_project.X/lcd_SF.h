/* Header file for LCD Display */
//Define LCD Display Signals - change these if you use different pins
#define DB7 LATBbits.LATB10 // pin 21
#define DB6 LATBbits.LATB7 // pin 16
#define DB5 LATBbits.LATB6 // pin 15
#define DB4 LATBbits.LATB5 // pin 14
#define ENABLE  LATBbits.LATB14 // pin 25
#define RS  LATBbits.LATB11  // pin 22
#define RW  LATBbits.LATB13 // pin 24

#define LCD_BUS_DIR TRISB
#define EN_DIRECTION TRISBbits.TRISB14
#define RS_DIR TRISBbits.TRISB9
#define RW_DIR TRISBbits.TRISB13

void Init_LCD(void);
void ClearScreen(void);
void Pulse_Enable(void);
void WriteCMD(unsigned char Cmd);
void WriteData(unsigned char Data);
void WriteString_LCD(const char *String);