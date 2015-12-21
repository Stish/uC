// TX Module

#define AD_PIN_1 0			//Connect ADC to ad0
#define AD_PIN_2 1			//Connect ADC to ad1
#define F_CPU	16000000	//Board frequency

#include <avr/io.h>			//IO Ports library
#include <avr/interrupt.h>						//library for interrupt functions
#include "can_at90can128.c"
#include <util/delay.h>

void TIMER0_ini (void);

unsigned char ad_channel_select = 0;
unsigned char val_ad_01_bcd[6] = "5.00V";
unsigned char val_ad_02_bcd[6] = "5.00A";
unsigned long val_ad_01 = 0;						//ad value decimal, channel 1, voltage
unsigned long val_ad_02 = 0;						//ad value decimal, channel 2, voltage
unsigned char timer0_cnt = 0;
unsigned char send_data = 0;
unsigned char send_data_position = 0;

can_message msg;
can_message msg_rec;
can_message msg_2;

SIGNAL (SIG_OVERFLOW0)								//a/d converter in interrupt mode
{
	TCNT0 = 0x05;
	timer0_cnt++;
	if (timer0_cnt == 25) //100ms
	{
		msg.data[0] = val_ad_01_bcd[0];
		msg.data[1] = val_ad_01_bcd[2];
		msg.data[2] = val_ad_01_bcd[3];
		
		msg.data[4] = val_ad_02_bcd[0];
		msg.data[5] = val_ad_02_bcd[2];
		msg.data[6] = val_ad_02_bcd[3];
		
		msg_2.data[0] = 0;
		msg_2.data[1] = 1;
		msg_2.data[2] = 2;
		msg_2.data[3] = 3;
		/*msg_2.data[4] = 4;
		msg_2.data[5] = 5;
		msg_2.data[6] = 6;
		msg_2.data[7] = 7;
		*/
		//can_enable_mob(0, TRANSMIT_DATA, msg);
		can_send_data(0, msg.data, msg.length);
		//_delay_ms(10);
		//can_enable_mob(0, TRANSMIT_DATA, msg_2);
		can_send_data(1, msg_2.data, msg_2.length);
		//PORTE ^= (1 << 6); 
		timer0_cnt = 0;
		send_data = 1;
		UDR1 = val_ad_01_bcd[send_data_position];
		send_data_position++;
	}
}

SIGNAL (SIG_ADC)								//a/d converter in interrupt mode
{
	if (ad_channel_select == 0)
	{
		val_ad_01 = ADCL;
		val_ad_01 |= (ADCH<<8);
		ad_channel_select = 1;
		//ADCSRA &= ~(1 << ADEN);
		ADMUX = 0x40 | AD_PIN_1;
		//ADCSRA |= (1 << ADEN);
	}
	else if (ad_channel_select == 1)
	{
		val_ad_02 = ADCL;
		val_ad_02 |= (ADCH<<8);
		ad_channel_select = 0;
		//ADCSRA &= ~(1 << ADEN);
		ADMUX = 0x40 | AD_PIN_2;
		//ADCSRA |= (1 << ADEN);
	}
}

int main(void)								//main program
{
	unsigned short val_ad_01_buffer = 0;
	unsigned short val_ad_02_buffer = 0;
	//unsigned char i = 0;
	
	DDRE |= (1 << 6);
	PORTE |= (0x01 << 6);
	
	can_init(125, NONE);
	
	msg.id = 0x02345678;
	msg.idm = 0xffffffff;
	msg.length = 8;
	
	msg_2.id = 0x00000001;
	msg_2.idm = 0xffffffff;
	msg_2.length = 8;
	// Objekt 0 auf Empfang setzen
	
	can_enable_mob(0, TRANSMIT_DATA, msg);
	can_enable_mob(1, TRANSMIT_DATA, msg_2);
	
	PORTE &= ~(0x01 << 6);
	PORTE |= (0x01 << 6);

	sei();										//global interrupt enable on
	ADMUX = 0x40 | AD_PIN_1;					//connect to AD_PIN
	ADCSRA = 0x9e;								//enable ad converter
	ADCSRA |= 0x60;								//start converter

	TIMER0_ini();

	while(1)
	{
		val_ad_01_buffer = (val_ad_01 * 48828) / 100000;
		val_ad_01_bcd[0] = (val_ad_01_buffer / 100) + 0x30;
		val_ad_01_buffer = val_ad_01_buffer % 100;
		val_ad_01_bcd[2] = (val_ad_01_buffer / 10) + 0x30;
		val_ad_01_buffer = val_ad_01_buffer % 10;
		val_ad_01_bcd[3] = val_ad_01_buffer + 0x30;
		
		val_ad_02_buffer = (val_ad_02 * 48828) / 100000;
		val_ad_02_bcd[0] = (val_ad_02_buffer / 100) + 0x30;
		val_ad_02_buffer = val_ad_02_buffer % 100;
		val_ad_02_bcd[2] = (val_ad_02_buffer / 10) + 0x30;
		val_ad_02_buffer = val_ad_02_buffer % 10;
		val_ad_02_bcd[3] = val_ad_02_buffer + 0x30;
	}
}	

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x05;
}
