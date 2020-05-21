/**************************************************
; File: siren.c
; Description:
;    This file contains the code for the module siren. This modules 
;	 drives the PC speaker that mimics the siren.
**************************************************/
/*********************INCLUDES********************/
#include "mc9s12dg256.h"
#include "siren.h"
/*******************DEFINITIONS*******************/
#define FIVE_MS 3750	// 3750 x 1 1/3 = 5000 micro-second 
  
/*******************GLOBAL VARS*******************/

/*----------------------------------------------------
Function: initSiren
----------------------------------------------------*/
void initSiren()
{
	TIOS |= 0b00100000;  // Enable ouput compare channel 5
}

/*----------------------------------------------------
Function: turnOnSiren
----------------------------------------------------*/
void turnOnSiren()
{
   TCTL1 |= 0b00001100;  // Output Compare puts 5v on PT5
   CFORC = 0b00100000;   // Force the 5V on the pin
   TCTL1 &= 0b11110111;  // Set TC5 to toggle on OC: clear bit 3 so that TCTL1_OM5=0 and TCTL1_OL5 = 1
   TC5 = TCNT + FIVE_MS; // first interupt in 5ms
   TIE |= 0b00100000;    // Enable Interrupt for channel 5
   asm cli;
}

/*----------------------------------------------------
Function: turnOffSiren
----------------------------------------------------*/
void turnOffSiren()
{
   TIE &= 0b11011111;    // Disable Interrupt for channel 5
   TCTL1 &= 0b11110011;	 // Disconnet Timer from output pin logic
   TCTL1 |= 0b00001000;  // Set TC5 to zero on OC so that TCTL1_OM5=1 and TCTL1_OL5 = 0
   CFORC = 0b00100000;   // Force 0V on pin 5
}

/*----------------------------------------------------
Interrupt: tc5_isr
----------------------------------------------------*/
void interrupt VectorNumber_Vtimch5 tc5_isr (void)
{
TC5 += FIVE_MS;
}//END OF ISR

