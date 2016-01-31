
#include "msp432.h"

unsigned int autoState;
unsigned int colorState;

void selectPortFunction(int port, int line, int sel10, int sel1){
	//(p,l,0,0) will set port to Digital I/O
	if(port==1){
		if(P1SEL0 & BIT(line)!=sel10){
			if(P1SEL1 & BIT(line)!=sel1){
				P1SELC|=BIT(line);
			}else{
				P1SEL0^=BIT(line);
			}
		}else{
			if(P1SEL1 & BIT(line)!=sel1)
				P1SEL1^=BIT(line);
		}
	}else{
		if(P2SEL0 & BIT(line)!=sel10){
			if(P2SEL1 & BIT(line)!=sel1){
				P2SELC|=BIT(line);
			}else{
				P2SEL0^=BIT(line);
			}
		}else{
			if(P2SEL1 & BIT(line)!=sel1)
				P2SEL1^=BIT(line);
		}
	}
}

void initLED(void){
	P2DIR|=BIT0|BIT1|BIT2;  //1 aka "out" for LED2 on lines 0,1,2
	selectPortFunction(2,0,0,0);
	selectPortFunction(2,1,0,0);
	selectPortFunction(2,2,0,0);
}


void initButtons(void){
	P1DIR&=~(BIT1&BIT4); //0 aka "in" for button on lines 1,4

	P1REN|=BIT1|BIT4;  //enaling internal pull-up/pull-down resistors
	P1OUT|=BIT1|BIT4;  //default circuit to pull-ups

	selectPortFunction(1,1,0,0);
	selectPortFunction(1,4,0,0);
}

void newColor (){
	if(++(colorState)==8)
		colorState=0;
}

void setColor (){
	P2OUT&=0xF8;       //and with F8 to zero out bits 0,1,2
	P2OUT|=colorState; //or with the color to set 0,1,2 as appropriate
}

void PortOneInterrupt(void) {
	unsigned short iflag=P1IV; //IV=interrupt vector
	if(iflag==0x04)//if line 1 was hit (datasheet 10.4.1)
		autoState^=1;
	if(iflag==0x0A && !autoState){//if line 4 was hit (datasheet 10.4.1)
		setColor();
		newColor();
	}

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
	//init variables
	autoState = 1;
	colorState = 0;
	//Enter loop

		while(1){
			if(autoState){
				setColor();
				newColor();
			}
			volatile int k=0; //Using volatile to trick complier into letting empty loop run
			for (k = 0; k < 20000; ++k);
		}
}

