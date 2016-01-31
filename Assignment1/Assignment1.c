
#include "msp432.h"

void PortOneInterrupt(void) {
	unsigned short iflag=P1IV; //IV=interrupt vector
	P1OUT^=BIT0;
}

void selectDIO_P1(char bitToSet){
	//Set Port1, Line[bitToSet]'s selectors to both be zero (Digital I/O mode)
	if (P1SEL0 & bitToSet){
		if(P1SEL1 & bitToSet){
			P1SELC|=bitToSet;  //Both 1 => both to 0 using complement. 10.2.6 in user guide
		}else{
			P1SEL0&=~bitToSet;
		}
	} else if (P2SEL1 & bitToSet){
		P1SEL1&=~bitToSet;
	}
}

void initLED(void){
	P2DIR|=BIT0|BIT1|BIT2;  //1 aka "out" for LED2 on lines 0,1,2
	selectDIO_P2(BIT0);
	selectDIO_P2(BIT1);
	selectDIO_P2(BIT2);
}


void initButtons(void){
	P1DIR&=~(BIT1&BIT4); //0 aka "in" for button on lines 1,4

	P1REN|=BIT1|BIT4;  //enaling internal pull-up/pull-down resistors
	P1OUT|=BIT1|BIT4;  //default circuit to pull-ups

	selectDIO_P1(BIT1);
	selectDIO_P1(BIT4);
}

void newColor (unsigned int *colorState){
	if(++(*colorState)==8)
		*colorState=0;
}

void setColor (unsigned int colorState){
	P2OUT&=0xF8;       //and with F8 to zero out bits 0,1,2
	P2OUT|=colorState; //or with the color to set 0,1,2 as appropriate
}

void main(void){

	WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
	/*Initialize the I/O ports*/
	initLED();
	initButtons();
	//init Interrupts
	P1IE=(BIT1|BIT4);   //interrupt enable P1 lines 1,4 only
	P1IES=(BIT1|BIT4);  //interupt on a high-to-low transition (buttons are set pull-up)
	NVIC_EnableIRQ(PORT1_IRQn); //Enable P1 interrupts using the NVIC
								//NVIC=nested vector interrupt controller

	while(1){}
}

