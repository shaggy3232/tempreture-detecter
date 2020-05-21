/**************************************************
; File: thermistor.c
; Description:
;    Using Dragon Board A/D converter to implement
;    	thermistor to measure temperature 
**************************************************/
/*********************INCLUDES********************/
#include <stdio.h>
#include <stdtypes.h>
#include "mc9s12dg256.h"
#include "thermistor.h"
/*******************DEFINITIONS*******************/
#define FIFTY_MS  375000 // 375000 x 1 1/3 = 500000 micro-second 
/*******************GLOBAL VARS*******************/
int tempValue;
int MScnt; //number 50ms delay have occured
/*---------------------------------------------
Function: initThermistor
Description: initializes hardware for the 
             thermistor.
-----------------------------------------------*/
void initThermistor(void) 
{ 
int i;
	ATD0CTL2 = 0b11000010;// enable interruptions, fast clear,
	for ( i=120; i>0; i--);	// Wait 20 microsecs (delay 
                              // 480(=4x120) clock cyles)
							//		ldaa #120
							// dlylp	nop
							//		dbne a,dlylp
	
	ATD0CTL3 = 0b00001000;//single conversion no FIFO
	ATD0CTL4 = 0b00010111;// for conversion use 10-bit resolution 500khz clock cylce
	
	TIOS_IOS3 = 1;  //enable channel 3
	TC3 = TCNT + FIFTY_MS ; //first interruption in 50 ms
	TIE |= 0b00001000; //enable interrupt on channel 3    
	asm cli;
}


/*---------------------------------------------
Function: getTemp
Description: Transates 10 bit conversion value 
to interger between 000 and 700
-----------------------------------------------*/
int getTemp(void) 
{
return 4.88876*tempValue;	    
	    	   	  
}


void interrupt VectorNumber_Vtimch3 tc3_isr(void) {
  if (MScnt > 0) {  //wait for 100 ms 
       ATD0CTL5 = 0b10000101; //start ATD conversion seqeunce, single Pin on Pin 5 
       MScnt = 0;//reset number of 50 ms count
  }
  MScnt++;  
  TC3 = TCNT + FIFTY_MS; 
  TFLG1 &= 0b11110111;   //Flag clear, CH3
} 

void interrupt VectorNumber_Vatd0 atd_isr(void) { 
	tempValue = ATD0DR0; //assigns value from the ATD 
}
