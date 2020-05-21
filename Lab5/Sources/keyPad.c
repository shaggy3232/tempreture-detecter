/*
File: keyPad.c
DEscription: Keypad Module with interupt
*/

#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "SegDisp.h"
#define NOKEY 0
#define BIT4 0b00010000 // set bit 4 for channel 4
#define TimeMSEC 7500 // = 10 ms
#define WAITING_FOR_KEY 0
#define DEB_KEYPRESS    1
#define WAITING_FOR_REL 2
#define DEB_REL         3

// Global variables
volatile byte keyCode;

// Local Function
char getAscii(byte);
byte getKeyCode(void);

// this is the key code in binary
byte kCodes[16] =  { 0b11101110, 0b11101101, 0b11101011, 0b11100111, 
                   0b11011110, 0b11011101, 0b11011011, 0b11010111, 
                   0b10111110, 0b10111101, 0b10111011, 0b10110111, 
                   0b01111110, 0b01111101, 0b01111011, 0b01110111 
                   };

// this is the corresponding key code in ascii
byte aCodes[16] =  { '1',        '2',        '3',        'a',    
                   '4',        '5',        '6',        'b',
                   '7',        '8',        '9',        'c',
                   '*',        '0',        '#',        'd' 
                   };


/*
 InitKeyPad
*/

void initKeyPad(void) 
{
	DDRA   = 0xF0; // PORTA all outputs : DDRA  = 1111 0000 --> 1 = input and 0 = ouput : -> PA7-PA4 are inputs and PA3-PA0 are outputs
	PORTA  = 0x00; // initialize PORTA  : PORTA = 0000 0000
	PUCR |= 0x01;  // initialize PUCR   : PUCR  = 0000 0001 --> pull up resistor now : -> 0 = input and 1 = output : PA7-PA4 are outputs and PA3-PA0 are intputs
	TIOS |= BIT4;  // set output compare for TC4 -- channel 4 : TIOS is timer input capture/output compare select register ... where 0 = channel acts as an input capture and  1 = channel acts as an output compare
	TIE |= BIT4;   // enable interrupt on TC4 -- enable channel 4 : TIE is timer Interrupt Enable Register
	TC4 = TCNT + TimeMSEC; // TCNT + 10ms (in pulses) : TC4 is timer input Capture/Output Compare Register 4
	keyCode = NOKEY;	  
}

/*
 pollReadKey
*/

char pollReadKey() 
{
    char ch;
    if(keyCode == 0) {
      ch = 0;
    }
    else
    {  
        ch = getAscii(keyCode); // convert to ascii
        keyCode = 0; // re-initialize keyCode to null
    }
    return(ch);     
}


/*
 readKey
*/

char readKey(void) 
{
	   char ch;
	   while (keyCode == 0) 
	   {     
	     //Do nothing
	   }
	   ch = getAscii(keyCode);
	   keyCode = 0; // re-initialize keyCode to null
	   return (ch);	   	  
}

/*
 key_ISR : interrupt serive checks this fonction every 10 ms
*/

void interrupt VectorNumber_Vtimch4 key_isr(void)
{
  static byte state = WAITING_FOR_KEY; 
  static byte code;

	switch(state) 
  {
    case WAITING_FOR_KEY: 
      code = PORTA;
      if(code != 0x0F) state = DEB_KEYPRESS; // if code is not 0000 1111 , it means that it was pressed, thus the state changes to DEB_KEYPRESS to check if it was really pressed and too get actual code value
      break;
    case DEB_KEYPRESS:   
      if(PORTA != code) state = WAITING_FOR_KEY;  // if the value of code is different then the value at port a then it means that the key was not pressed... just noise, thus the state changes back too WAITING_FOR_KEY
      else 
      {
         code = getKeyCode(); // otherwise get the key code
         state = WAITING_FOR_REL; // state changes too WAITING_FOR_REL, which is the release of key wait    
      }
      break;
    case WAITING_FOR_REL: 
      if(PORTA == 0x0F) state = DEB_REL;  // if port a is 0000 1111, then the key was released, state changes to DEB_REL to check if it was really realese
      break;
    case DEB_REL:
      if(PORTA != 0x0F) state = WAITING_FOR_REL; // if port a is not 0000 1111, then it means it was not realesed earlier, it was just noise
      else 
      {
          keyCode = code;  
          state = WAITING_FOR_KEY; 
      }
      break;
  }

	TC4 = TC4 + TimeMSEC; // initialize next interrupt too create delay
}


/*
getKeyCode
*/

#define ROW1 0b11101111
#define ROW2 0b11011111
#define ROW3 0b10111111
#define ROW4 0b01111111

byte getKeyCode()
{
  volatile byte code;
 
  PORTA = ROW1;
  if( PORTA == ROW1)
  {
     PORTA = ROW2;
     if( PORTA == ROW2)
     {
        PORTA = ROW3;
        if( PORTA == ROW3) 
        {
           PORTA = ROW4;  
        }
     }
  } 
  code = PORTA;
  PORTA = 0x00; 
  return(code);
}

/*
getAscii
*/

char getAscii(byte cd)
{
    int i;
    char ch = 0;
    for(i=0; i < 16 ; i++)    
    {
       if(kCodes[i] == cd) // finds the key code in binary from the byte passed as argument     
       {
          ch = aCodes[i];  // then finds the corresponding ascii code corresponding to the binary code   
          break;
       }
    }
    return(ch);
}