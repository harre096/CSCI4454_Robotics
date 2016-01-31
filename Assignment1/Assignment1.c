
#include "msp432.h"

//unsigned int colorState;
//unsigned int blackOut;

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

void setClockFrequency(void){
	CSKEY=0x695A;       //unlock
	CSCTL1=0x00000223;  //run at 128, enable stuff for clock
	CSCLKEN=0x0000800F;
	CSKEY=0xA596;       //lock
}

void configureTimer(void){
	TA0CTL=0x0100;   //Picks clock (above), count up mode, sets internal divider, shuts timer off.


	TA0CCTL0=0x2000; //Pick compare (not capture) register, interrupt off
	TA0CCR0=0x0080;  //(128)//sets the max time compare register (1,2,3 depend on this peak)
					 //interrups every milisecond

	TA0CCTL1=0x2010; //Pick compare (not capture) register, interrupt on
	TA0CCR1=0x0080;   //sets the max time compare  for this capture, will wait until overflow


	TA0CTL=0x0116;   //Sets counter to 0, turns counter on, enables timeout (aka overflow) interrups
}
//void newColor(){
//	P2OUT&=0xF8;       //and with F8 to zero out bits 0,1,2
//	if(!blackOut){
//		if(++(colorState)==8)
//			colorState=0;
//		P2OUT|=colorState; //or with the color to set 0,1,2 as appropriate
//	}
//	blackOut^=1;
//}

void TimerA0Interrupt(void) {
	static int intCycles=0;
	static unsigned short intensity=0x80;

	unsigned short intv=TA0IV; //IV=interrupt vector
	if(intv==0x0E){// OE is overflow interrupt
		if(++intCycles == 10){
			intCycles=0;
			if(intensity==0){
				intensity=128;
			}else{
				intensity-=1;
			}
			TA0CCR1=intensity;
		}
	P2OUT&=~BIT0;
	}else if(intv==0x02){
		P2OUT|=BIT0;
	}

}

void main(void){

	WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
	setClockFrequency();
	initLED();
	configureTimer();
	NVIC_EnableIRQ(TA0_N_IRQn); //Enable TA0 interrupts using the NVIC
								//NVIC=nested vector interrupt controller

	while(1){}
}

