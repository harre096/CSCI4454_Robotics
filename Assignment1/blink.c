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
/*void main(void){
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
}*/


/*Adding Button*/

void initLED(void){
	P1DIR|=BIT0;  //1 aka "out" for led on line 0

	//Set Port1 Line0's Selectors to both be zero
	if (P1SEL0 & BIT0){ //If bit zero is 1 SEL0
		if(P1SEL1 & BIT0){
			P1SELC|=BIT0;  //Both 1 => both to 0 using complement. 10.2.6 in user guide
		}else{
			P1SEL0&=~BIT0;
		}
	} else if (P1SEL1 & BIT0){ //If bit zero is 1 SEL1
		P1SEL1&=~BIT0;
	}

	P1OUT|=BIT0;  	//Turn LED on
}


void initButton(void){
	P1DIR&=~BIT1; //0 aka "in" for button on line 1

	/*Initially, with above code P1IN BIT1 is 1 or high. However, after the button is
	pressed, that line (P1.1) is grounded and set to low/0. The loop changes LED
	everytime thatP1.1 is grounded. So without further setup, pressing the button will
	leave BIT as 0 or low, forever after (ie. auto blink mode).
	Pull-up fix in next two lines.*/
	P1REN|=BIT1;  //enaling internal pull-up resistor
	P1OUT|=BIT1;  //since 0=buttonDown, default circuit as buttonUp aka 1

	if (P1SEL0 & BIT1){ //If bit zero is 1 SEL0
		if(P1SEL1 & BIT1){
			P1SELC|=BIT1;  //Both 1 => both to 0 using complement. 10.2.6 in user guide
		}else{
			P1SEL0&=~BIT1;
		}
	} else if (P1SEL1 & BIT1){ //If bit zero is 1 SEL1
		P1SEL1&=~BIT1;
	}
}

void main(void){
	WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer

	//Initialize the I/O port
	initLED();
	initButton();


	//Enter loop
	while(1){
		unsigned char portIn = P1IN;
		if(!(portIn & BIT1)) //if portIn BIT1 is 0, flip led bit
			P1OUT^=BIT0;
		volatile int k=0; //Using volatile to trick complier into letting empty loop run
		for (k = 0; k < 20000; ++k);
	}
}
