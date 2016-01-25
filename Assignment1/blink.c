//*****************************************************************************
//
// MSP432 main.c template - P1.0 port toggle
//
//****************************************************************************

#include "msp432.h"
/*
 *Online Example:
void main(void)
{
    volatile uint32_t i;

    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    // The following code toggles P1.0 port to 0x01
    P1DIR |= BIT0;                      // Configure P1.0 as output

    while(1)
    {
        P1OUT ^= BIT0;                  // Toggle P1.0
        for(i=30000; i>0; i--);         // Delay
    }
    P1DIR |= BIT0;

}*/

/*  Kevin Example */
void main(void){
	WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
	// Initiaze the I/O port
	P1DIR|=BIT0;

	//Set Port 1's Selectors to both be zero
	if (P1SEL0 & BIT0){ //If bit zero is 1 SEL0
		if(P1SEL1 & BIT0){
			P1SELC|=BIT0;  //Both are 1, change both to zero using complement.
			// This is janky imo. See 10.2.6 in user guide :)
		}else{
			P1SEL0&=~BIT0;
		}
	} else if (P1SEL1 & BIT0){ //If bit zero is 1 SEL1
		P1SEL1&=~BIT0;
	}

	//Turn LED on
	P1OUT|=BIT0;

	//Enter loop
	while(1){
		volatile int k=0; //Using volatile to trick complier into letting empty loop run
		for (k = 0; k < 20000; ++k);
		P1OUT^=BIT0; //Use Xor to flip bit0
	}

}
