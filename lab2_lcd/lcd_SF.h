/* Header file for LCD Display */
//Define LCD Display Signals - change these if you use different pins
#define DB7 LATBbits.LATB8
#define DB6 LATBbits.LATB7
#define DB5 LATBbits.LATB6
#define DB4 LATBbits.LATB5
#define ENABLE  LATBbits.LATB14   
#define RS  LATBbits.LATB9 
#define RW  LATBbits.LATB13

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