#include "main.h"

int main(void)
{
	DDRB |= (0x01 << PB0);
	can_init(125, RX);
	
	msg_test.id_type = 1;	//type A, 11bit ID
	msg_test.id = 0x111;
	msg_test.idm = 0x7ff;
	msg_test.length = 7;
	msg_test.data[0] = 'T';
	msg_test.data[1] = 'e';
	msg_test.data[2] = 's';
	msg_test.data[3] = 't';
	msg_test.data[4] = '1';
	msg_test.data[5] = '2';
	msg_test.data[6] = '3';
	msg_test.data[7] = '4';

	msg_bat_1.id_type = 0;
	msg_bat_1.id = 0x030;
	msg_bat_1.idm = 0x7ff;	
	msg_bat_1.length = 8;
	
	msg_bat_2.id_type = 0;
	msg_bat_2.id = 0x031;
	msg_bat_2.idm = 0x7ff;	
	msg_bat_2.length = 8;

	can_enable_mob (10, TRANSMIT_DATA, msg_test);
	can_enable_mob (0, RECEIVE_DATA, msg_bat_1);
	can_enable_mob (1, RECEIVE_DATA, msg_bat_2);
	
	sei();
	TIMER0_ini();
	USART0_ini();
	

	while(1)
	{
	}
}

SIGNAL (SIG_UART0_TRANS)								//a/d converter in interrupt mode
{
	if (send_data == 1)
	{
		if (send_data_position < 31)
		{
			//UDR0 = msg_bcd[send_data_position];
			UDR0 = msg_uart30[send_data_position];
			send_data_position++;
		}
		else
		{
			UDR0 = 0x0d;
			send_data_position = 0;
			send_data = 0;
		}
	}
}

SIGNAL (SIG_OVERFLOW0)								//a/d converter in interrupt mode
{
	TCNT0 = 0x06;
	timer0_cnt++;
	
	if (timer0_cnt == 250) //100ms
	{
		send_data = 1;
		send_data_position = 0;
		UDR0 = START_OF_MESSAGE;
			
		can_send_data (10, msg_test);
		timer0_cnt = 0;
		//can_rec_data (1, msg_bat);
	}
}

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x06;
}

void USART0_ini (void)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (BAUD>>8);
	UBRR0L = (unsigned char) BAUD;
	UCSR0C = (0 << UMSEL0) | //asynchronous
			(0 << UPM00) | (0<<UPM01) |	//Mode 00: no parity
			(0 << USBS0) | 	//1 stop bit
			(1 << UCSZ01) | (1 << UCSZ00);	//Mode 011: 8 bit data
	/* Enable transmitter, disable receiver */
	UCSR0B = (0 << RXCIE0) | (1 << TXCIE0) | (0 << RXEN0) | (1 << TXEN0) | (0 << UCSZ02);
	UCSR0A = (0 << U2X0);
}

SIGNAL (SIG_CAN_INTERRUPT1)
{
	//PORTE &= ~(0x01 << 6);
	uint8_t		save_canpage;
 	static		can_message message;

	// Aktuelle CANPAGE sichern
 	save_canpage	= CANPAGE;

    // Index des MOB ermitteln, der den Interrupt ausgelöst hat
	uint8_t mob 	= can_get_mob_interrupt();
    
	// Falls es kein gültiges MOB war abbrechen
	if(mob == NOMOB)
	{
		return;
	}

	// Objekt das den Interrupt ausgelöst hat holen
	can_select_mob(mob);
    
	// Daten des MOBs aus CANMSG auslesen
	message			= can_get_mob_data();
	
	// Id der Nachricht holen
	message.id		= can_get_mob_id(can_get_mob_id_type());

	PORTB ^= (0x01 << PB0);
//	//////////////////////////////////
//	// Daten verarbeiten / ausgeben //
//	//////////////////////////////////
//
	if (message.id == 0x030)
	{
		// Cell 1
		msg_uart30[1] = message.data[1];	// H Byte
		msg_uart30[2] = message.data[0];	// L Byte
		// Cell 2
		msg_uart30[3] = message.data[3];	// H Byte
		msg_uart30[4] = message.data[2];	// L Byte
		// Cell 3
		msg_uart30[5] = message.data[5];	// H Byte
		msg_uart30[6] = message.data[4];	// L Byte
		// Cell 4
		msg_uart30[7] = message.data[7];	// H Byte
		msg_uart30[8] = message.data[6];	// L Byte
	}
	if (message.id == 0x031)
	{
		// Cell 5
		msg_uart30[9] = message.data[1];	// H Byte
		msg_uart30[10] = message.data[0];	// L Byte
	}

//	//////////////////////////////////
//	// Daten verarbeiten / ausgeben //
//	//////////////////////////////////

	// RXOK-Flag löschen
	clearbit(CANSTMOB, RXOK);
    
	// MOB auf Empfang und CAN 2.0B Standard setzen
	can_set_mob_mode(RECEIVE_DATA, can_get_mob_id_type());
    
	//CANPAGE wiederherstellen
	CANPAGE		= save_canpage;
}
/*
void can_rec_data(uint8_t mob, can_message message)
{
	//PORTE &= ~(0x01 << 6);
	uint8_t		save_canpage;
 	//can_message message;
	uint16_t ad_val_buffer = 0;

	// Aktuelle CANPAGE sichern
 	save_canpage	= CANPAGE;

    // Index des MOB ermitteln, der den Interrupt ausgelöst hat
	//uint8_t mob 	= can_get_mob_interrupt();
    
	// Falls es kein gültiges MOB war abbrechen
	//if(mob == NOMOB)
	//{
		//return;
	//}

	// Objekt das den Interrupt ausgelöst hat holen
	can_select_mob(mob);
    
	// Daten des MOBs aus CANMSG auslesen
	message			= can_get_mob_data();
	
	// Id der Nachricht holen
	message.id		= can_get_mob_id(can_get_mob_id_type());

	PORTB ^= (0x01 << PB0);
//	//////////////////////////////////
//	// Daten verarbeiten / ausgeben //
//	//////////////////////////////////
//
	if ((message.id_type == 0) && (message.id == 0x030))
	{
		msg_bcd[4] = '3';
			
		msg_uart[1] = message.data[1];	// H Byte
		msg_uart[2] = message.data[0];	// L Byte
		
		ad_val_buffer = ( (uint16_t) message.data[1] << 8) + (uint16_t) message.data[0];
		
		msg_bcd[13] = (ad_val_buffer / 1000) + 48;
		ad_val_buffer -= (ad_val_buffer / 1000) * 1000;
		msg_bcd[14] = (ad_val_buffer / 100) + 48;
		ad_val_buffer -= (ad_val_buffer / 100) * 100;
		msg_bcd[15] = (ad_val_buffer / 10) + 48;
		ad_val_buffer -= (ad_val_buffer / 10) * 10;
		msg_bcd[16] = (ad_val_buffer) + 48;
		
		ad_val_buffer = ( (uint16_t) message.data[3] << 8) + (uint16_t) message.data[2];
		
		msg_bcd[24] = (ad_val_buffer / 1000) + 48;
		ad_val_buffer -= (ad_val_buffer / 1000) * 1000;
		msg_bcd[25] = (ad_val_buffer / 100) + 48;
		ad_val_buffer -= (ad_val_buffer / 100) * 100;
		msg_bcd[26] = (ad_val_buffer / 10) + 48;
		ad_val_buffer -= (ad_val_buffer / 10) * 10;
		msg_bcd[27] = (ad_val_buffer) + 48;
		
		ad_val_buffer = ( (uint16_t) message.data[5] << 8) + (uint16_t) message.data[4];
		
		msg_bcd[35] = (ad_val_buffer / 1000) + 48;
		ad_val_buffer -= (ad_val_buffer / 1000) * 1000;
		msg_bcd[36] = (ad_val_buffer / 100) + 48;
		ad_val_buffer -= (ad_val_buffer / 100) * 100;
		msg_bcd[37] = (ad_val_buffer / 10) + 48;
		ad_val_buffer -= (ad_val_buffer / 10) * 10;
		msg_bcd[38] = (ad_val_buffer) + 48;
		
		ad_val_buffer = ( (uint16_t) message.data[7] << 8) + (uint16_t) message.data[6];
		
		msg_bcd[46] = (ad_val_buffer / 1000) + 48;
		ad_val_buffer -= (ad_val_buffer / 1000) * 1000;
		msg_bcd[47] = (ad_val_buffer / 100) + 48;
		ad_val_buffer -= (ad_val_buffer / 100) * 100;
		msg_bcd[48] = (ad_val_buffer / 10) + 48;
		ad_val_buffer -= (ad_val_buffer / 10) * 10;
		msg_bcd[49] = (ad_val_buffer) + 48;

		//msg_bcd[11] = 'a';
		//msg_bcd[12] = 'b';
	}

//	//////////////////////////////////
//	// Daten verarbeiten / ausgeben //
//	//////////////////////////////////

	// RXOK-Flag löschen
	clearbit(CANSTMOB, RXOK);
    
	// MOB auf Empfang und CAN 2.0B Standard setzen
	can_set_mob_mode(RECEIVE_DATA, can_get_mob_id_type());
    
	//CANPAGE wiederherstellen
	CANPAGE		= save_canpage;
}*/