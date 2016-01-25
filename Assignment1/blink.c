//*****************************************************************************
//
// MSP432 main.c template - P1.0 port toggle
//
//****************************************************************************

#include "msp432.h"

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
}
