#include "msp432.h"

void setSelectToZeroP1(char bitToSet){
	//Set Port1, Line[bitToSet]'s Selectors to both be zero (Digital I/O mode)
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
void setSelectToZeroP2(char bitToSet){
	//Set Port2, Line[bitToSet]'s Selectors to both be zero (Digital I/O mode)
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
	setSelectToZeroP2(BIT0);
	setSelectToZeroP2(BIT1);
	setSelectToZeroP2(BIT2);
}


void initButtons(void){
	P1DIR&=~(BIT1&BIT4); //0 aka "in" for button on line 1 and 4

	P1REN|=BIT1|BIT4;  //enaling internal pull-up/pull-down resistor
	P1OUT|=BIT1|BIT4;  //default circuit to pull-up

	setSelectToZeroP1(BIT1);
	setSelectToZeroP1(BIT4);
}

void newColor (unsigned int colorState){


}

void main(void){
	WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
	initLED();
	initButtons();


	//Enter loop
	unsigned int autoModeState = 1;
	unsigned int colorState = 0;

	while(1){
		unsigned char portIn = P1IN;
		if(!(portIn & BIT1)){ //if portIn BIT1 is 0, toggle automatic color changing
			autoModeState^=1;
		}

		if(autoModeState){
			P2OUT&=0xF8;       //and with F8 to zero out bits 0,1,2
			P2OUT|=colorState; //or with the color to set 0,1,2 as appropriate
			colorState==8?colorState=0:colorState++; //increment or reset color
		} else if (!(portIn & BIT4)){ //if portIn BIT4 is 0, change color manually
			P2OUT&=0xF8;       //and with F8 to zero out bits 0,1,2
			P2OUT|=colorState; //or with the color to set 0,1,2 as appropriate
			colorState==8?colorState=0:colorState++; //increment or reset color
		}

		volatile int k=0; //Using volatile to trick complier into letting empty loop run
		for (k = 0; k < 20000; ++k);
	}
}
