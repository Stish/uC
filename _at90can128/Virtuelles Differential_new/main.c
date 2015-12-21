// TX Module

#define AD_PIN_0 0			//Connect ADC to adc0
#define AD_PIN_1 1			//Connect ADC to adc1
#define F_CPU	16000000	//Board frequency

#include <avr/io.h>			//IO Ports library
#include <avr/interrupt.h>						//library for interrupt functions
#include "can_at90can128.c"
#include <util/delay.h>


unsigned char ad_channel_select = 0;
unsigned long ulspeed = 0;						//ad value decimal, channel 1, voltage
unsigned long ulangle = 0;						//ad value decimal, channel 2, voltage
unsigned char timer0_cnt = 0;
unsigned char change = 0;
unsigned char direction = 0;
unsigned long overflow =0;
unsigned long speed_left = 0;
unsigned long speed_right = 0;

void PWM_TIM1_Init (void) 
{ 
	TCCR0A = (1<<CS01)|(0<<CS00); 
	TIMSK0 = 1<<TOIE0;
} 


SIGNAL (SIG_OVERFLOW0)
{
	overflow++;
	
	if (overflow >= speed_left)
	{
		PORTB &= ~(1<<5);
	}
	else
	{
		PORTB |= (1<<5);
	}	
	
	if (overflow>=speed_right)	
	{
		PORTB &= ~(1<<6);
	}
	else
	{
		PORTB |= (1<<6);
	}
	
	if (overflow == 100)
	{
		overflow = 0;
	}
}

SIGNAL (SIG_ADC)								//A/D converter in interrupt mode
{	
	if (ad_channel_select == 0)					// Geschwindigkeit (ADC 1), EXT1 - 17
	{
		ulspeed = ADCL;
		ulspeed |= (ADCH<<8);
		//ulspeed &= 0x3ff; 
		ad_channel_select = 1;
		//ADCSRA &= ~(1 << ADEN);		
		ADMUX = 0x40 | AD_PIN_0;
		//ADCSRA |= (1 << ADEN);

	}
	else if (ad_channel_select == 1)			//Lenkeinschlag (ADC 0), EXT1 - 16
	{
		ulangle = ADCL;
		ulangle |= (ADCH<<8);
		//ulangle &= 0x3ff; 
		ad_channel_select = 0;		
		//ADCSRA &= ~(1 << ADEN);
		ADMUX = 0x40 | AD_PIN_1;
		//ADCSRA |= (1 << ADEN);
	}
/*	
	if ((ulangle>=0)&&(ulangle<512))
 	{
 		change = 1;
		direction = 0;
 	}
 	else if ((ulangle>=512))
 	{
 		change = 1;
		direction = 1;
 	}
*/	

	if ((ulangle>=0)&&(ulangle<128))
 	{
 		change = 4;
		direction = 0;
 	}
 	else if ((ulangle>=128)&&(ulangle<256))
 	{
 		change = 3;
		direction = 0;
 	}
 	else if ((ulangle>=256)&&(ulangle<384))
 	{
 		change = 2;
		direction = 0;
 	}
 	else if ((ulangle>=384)&&(ulangle<512))
 	{
 		change = 1;
		direction = 0;
 	}
 	else if ((ulangle>=512)&&(ulangle<640))
 	{
 		change = 1;
		direction = 1;
 	}
 	else if ((ulangle>=640)&&(ulangle<768))
 	{
 		change = 2;
		direction = 1;
 	}
 	else if ((ulangle>=768)&&(ulangle<896))
 	{
 		change = 3;
		direction = 1;
 	}
 	else if ((ulangle>=896))
 	{
 		change = 4;
		direction = 1;
 	}

	if (direction == 0)			//Lenkrad nach links eingeschlagen
	{
	 	PORTE &= ~(0x01 << 4);											//Status LED on
		speed_right = (ulspeed / 10);
		speed_left = ((ulspeed * (10 - change)) / 100);
	}
	else if (direction == 1)	//Lenkrad nach rechts eingeschlagen
	{
		PORTE |= (0x01 << 4);											//Status LED off
		speed_right = ((ulspeed * (10 - change)) / 100);
		speed_left = (ulspeed / 10);
	}
}




int main(void)								//main program
{
	
	DDRB |= (1 << 6)|(1 << 5);		//enable OC1B (PB6) & OC1A (PB5) as output
	DDRE |= (1 << 4);


	sei();										//global interrupt enable on
	ADMUX = 0x40 | AD_PIN_0;					//connect to AD_PIN
	ADCSRA = 0x8f;								//enable ad converter
	ADCSRA |= 0x60;								//start converter

	PWM_TIM1_Init();
	

	while(1)
	{
	
	}
}	

