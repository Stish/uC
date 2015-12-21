#ifndef DELAY_C
#define DELAY_C
#include <avr/io.h>

/*
	With these function you can wait a specific amount of time

	Limitations:
	Timer 1 is used to measure the waiting time polling mode and
	therefor not available to your own program.

*/

//function waits 1-4500ms and returns
//its using timer 1 for waiting in polling mode
//ms: 1-4500
void delay_ms(unsigned int ms);

//function waits 1-65535 s and returns
//its using timer 1 for waiting in polling mode
//s: 1-65535
void delay_s(unsigned int s);



/**********************end of header ************************************/


void delay_ms(unsigned int ms){
	if((ms==0)||(ms>4500)) return;
	TCCR1B=0; //stop a running timer
	TIFR|=4;  //reset the overflow flag
	TIMSK&=~4;  //delete the interrupt enable for this timer
	TCCR1A=0;
	TCCR1C=0;
	TCNT1=65536-(((long)ms*144)/10);
	TCCR1B=5; //prescalar 1024
	while(!(TIFR&4));
	TCCR1B=0;  //stop the timer
	TIFR|=4;  //reset the overflow flag
}


//function waits 1-4500ms and returns
//its using timer 1 for waiting in polling mode
//ms: delays for 1-4500 ms
void delay_s(unsigned int s){
	while(s!=0){
		delay_ms(1000);s--;
	}
}


#endif