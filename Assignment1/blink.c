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

/*  Kevin Example
void main ( void)
{
	WDTCTL = WDTPW | WDTHOLD; // S top wa tchdog t ime r
	///∗ I n i t i a l i z e t h e I /O p o r t ∗/
	P1DIR|=BIT0 ;
	if (P1SEL0 & BIT0 )
	{
		if (P1SEL1 & BIT0 )
		P1SELC|=BIT0 ;
		else
		P1SEL0&=~BIT0 ;
	}
	else if (P1SEL1 & BIT0 )
	P1SEL1&=~BIT0 ;
	P1OUT|=BIT0 ; // t u rn t h e LED on
	///∗ Now we e n t e r t h e l o o p ∗/
	for ( ; ; ) // i d i om a t i c i n f i n i t e l o o p ; w h i l e ( 1 ) i s a l s o u s a b l e he re
	{
		volatile int k=0;
		for ( k=0;k<100000;++k ) ;
		P1OUT^=BIT0 ;
	}
}*/
