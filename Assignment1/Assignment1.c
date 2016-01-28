
#include "msp432.h"

void PortOneInterrupt(void) {
	unsigned short iflag=P1IV; //IV=interrupt vector
	P1OUT^=BIT0;
}

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
	})

void initLED(void){
	P1DIR|=BIT0;  //1 aka "out" for LED1 on lines 0
	selectPortFunction(1,0,0,0);
	P1OUT|=BIT0;
}

void setClockFrequency(void){
	CSKEY=0x695A;
	CSCTL1=0x00000223;
	CSCLKEN=0x0000800F;
	CSKEY=0xA596;
}

void configureTimer(void){
	TA0CTL=0x0100;
	TA0CCTL0=0x2000
	TA0CCR0=0xFA00; //or TA0CCRO=64000
	TA0TL=0x0116;
}

void initButtons(void){
	P1DIR&=~(BIT1&BIT4); //0 aka "in" for button on lines 1,4

	P1REN|=BIT1|BIT4;  //enaling internal pull-up/pull-down resistors
	P1OUT|=BIT1|BIT4;  //default circuit to pull-ups

	selectDIO_P1(BIT1);
	selectDIO_P1(BIT4);
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

