
#include "msp432.h"

struct color{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};
struct color myColors[9];
unsigned int colorState;
static int cycles;

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
	TA0CCR0=0x80;  //(128)//sets the max time compare register (1,2,3 depend on this peak)
					 //interrups every milisecond

	TA0CCTL1=0x2010; //Pick compare (not capture) register, interrupt on
	TA0CCR1=0x0040;   //sets the max time compare  for this capture, will wait until overflow (will be overwritten)
	TA0CCTL2=0x2010;
	TA0CCR2=0x0040;
	TA0CCTL3=0x2010;
	TA0CCR3=0x0040;

	TA0CTL=0x0116;   //Sets counter to 0, turns counter on, enables timeout (aka overflow) interrups
}

void TimerA0Interrupt(void) {
	unsigned short intv=TA0IV; //IV=interrupt vector
	if(intv==0x0E){// OE is overflow interrupt
		P2OUT&=0xF8;
		TA0CCR1=myColors[colorState].red;
		TA0CCR2=myColors[colorState].green;
		TA0CCR3=myColors[colorState].blue;

		if(++(cycles)==1000){
			cycles=0;
			if(++(colorState)==3)
				colorState=0;
		}

	}else if(intv==0x02){//red
		P2OUT|=BIT0;
	}else if(intv==0x04){//green
		P2OUT|=BIT1;
	}else if(intv==0x06){//blue
		P2OUT|=BIT2;
	}

}


void main(void){

	WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
	setClockFrequency();
	initLED();
	configureTimer();
	NVIC_EnableIRQ(TA0_N_IRQn); //Enable TA0 interrupts using the NVIC
								//NVIC=nested vector interrupt controller

	myColors[0].red=0x80;
	myColors[0].green=0x00;
	myColors[0].blue=0x4F;
	myColors[1].red=0x2F;
	myColors[1].green=0x0F;
	myColors[1].blue=0x00;
	myColors[2].red=0x16;
	myColors[2].green=0x80;
	myColors[2].blue=0x37;

	myColors[0].red=0x80;
	myColors[0].green=0x00;
	myColors[0].blue=0x4F;
	myColors[1].red=0x2F;
	myColors[1].green=0x0F;
	myColors[1].blue=0x00;
	myColors[2].red=0x16;
	myColors[2].green=0x80;
	myColors[2].blue=0x37;

	myColors[0].red=0x80;
	myColors[0].green=0x00;
	myColors[0].blue=0x4F;
	myColors[1].red=0x2F;
	myColors[1].green=0x0F;
	myColors[1].blue=0x00;
	myColors[2].red=0x16;
	myColors[2].green=0x80;
	myColors[2].blue=0x37;

	colorState=0;
	cycles=0;

	while(1){}
}

