
#include "msp432.h"

void InterruptHandler(void) ;

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
void selectDIO_P2(char bitToSet){
	//Set Port2, Line[bitToSet]'s selectors to both be zero (Digital I/O mode)
	if (P2SEL0 & bitToSet){
		if(P2SEL1 & bitToSet){
			P2SELC|=bitToSet;  //Both 1 => both to 0 using complement. 10.2.6 in user guide
		}else{
			P2SEL0&=~bitToSet;
		}
	} else if (P2SEL1 & bitToSet){
		P2SEL1&=~bitToSet;
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
	initLED();
	initButtons();


	//Enter loop
	unsigned int autoState = 1;
	unsigned int colorState = 0;

	while(1){
		unsigned char portIn = P1IN;
		if(!(portIn & BIT1)){ //if portIn BIT1 is 0, toggle automatic color changing
			autoState^=1;
		}

		if(autoState){
			setColor(colorState);
			newColor(&colorState);
		} else if (!(portIn & BIT4)){ //if portIn BIT4 is 0, change color manually
			setColor(colorState);
			newColor(&colorState);
		}

		volatile int k=0; //Using volatile to trick complier into letting empty loop run
		for (k = 0; k < 20000; ++k);
	}
}

