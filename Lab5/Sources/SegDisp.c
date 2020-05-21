/**************************************************
File: SegDisp.c
Description:  Segment Display Module
              Uses Timer Channel 1
**************************************************/
/*********************INCLUDES********************/
#include "mc9s12dg256.h"
#include "SegDisp.h"

/*******************DEFINITIONS*******************/
#define REFRESH_TIME 3750	// For 50 ms delay (1 1/3 microSec/tick).

/*******************GLOBAL VARS*******************/
byte characters[4];  // ASCII codes for each of the 4 display
static byte dispNum = 0;
struct ascii_to_code 
{
   byte ascii;
   byte code;  
}; 

struct ascii_to_code tbl[18] = 
{       
    { '0',0b00111111 },
    { '1',0b00000110 },
    { '2',0b01011011 },
    { '3',0b01001111 },
    { '4',0b01100110 },
    { '5',0b01101101 },
    { '6',0b01111101 },
    { '7',0b00000111 },
    { '8',0b01111111 },
    { '9',0b01101111 },
    { '*',0b01000110 },
    { '#',0b01110000 },
    { 'a',0b01110111 },
    { 'b',0b01111100 },
    { 'c',0b00111001 },
    { 'd',0b01011110 },
    { 'e',0b01111001 },
    { ' ',0b00000000 }  // space
};

byte enableDisp[4]= {   // display numbers
     0b00001110,	// display 0
     0b00001101,	// display 1
     0b00001011,	// display 2
     0b00000111	  // display 3
};

byte getCode(byte ch);
/*---------------------------------------------
Function: initDisp
Description: initializes hardware for the 
             7-segment displays.
-----------------------------------------------*/
void initDisp(void) 
{
	DDRB = 0xFF;  // output for PORT B
	DDRP |= 0x0F;  //output for PORT P 
	PTP |= 0x0F;   // Disables the displays
	clearDisp();  
	TIOS |= 0b00000010; //ouput compare for Channel 1
	TIE |= 0b00000010; // Enable interrupt on Channel 1
	
	TC1 = TCNT + REFRESH_TIME; //Refreshes displays  
}

/*---------------------------------------------
Function: clearDisp
Description: Clears all displays.
-----------------------------------------------*/
void clearDisp(void) 
{
   int i;
   for(i=0 ; i<4 ; i++)
      characters[i] = 0;
}

/*---------------------------------------------
Function: setCharDisplay
Description: Translates an ASCII character (ch)
             its corresponding bits to 
             display on 7-segment display. The 
             Bits are stored in the character 
             array.  
-----------------------------------------------*/
void setCharDisplay(char ch, byte dispNum) 
{
  byte code;
  code = getCode(ch);
  characters[dispNum] = code;
}

/*---------------------------------------------
Function: getCode
Description: Translates an ASCII character code
             (ch) to a 7-segment display code. 
             Returns 0 (blank) if character is
             not in the table;
-----------------------------------------------*/
byte getCode(byte ch) 
{
   byte code = 0;
   byte i;
   for(i=0 ; i < 18 && code==0 ; i++)
     if(ch == tbl[i].ascii) 
        code = tbl[i].code;
   return(code);  
}

/*---------------------------------------------
Function: turnOnDP
Description: Turns on the decimal point of the
				display identified by dNum
-----------------------------------------------*/
void turnOnDP(int dNum) 
{
    characters[dNum] = characters[dNum] | 0b10000000;  // sets bit 7  
}

/*---------------------------------------------
Function: turnOffDP
Description: Turns off the decimal point of 2nd
             display from the left.
-----------------------------------------------*/
void turnOffDP(int dNum) 
{
    characters[dNum] = characters[dNum] & 0b01111111;  // clears bit 7  
}

/*-------------------------------------------------
Interrupt: disp_isr
Description: Display interrupt service routine 
				updats displays every 50 ms.
---------------------------------------------------*/
void interrupt VectorNumber_Vtimch1 disp_isr(void)
{
	byte enable;
	PORTB = characters[dispNum]; // pass character to port b
	enable = PTP;  // get current values
	enable &= 0xF0; // erase lower four bits
	PTP = enable | enableDisp[dispNum++]; // set lower for bits
	dispNum = dispNum%4;
	TC1 = TC1 + REFRESH_TIME;
}



