// BMS - VCU Gateway
#define BAUD 3	//UBRR0 for 16MHz, U2X0 = 0 -> 250k Baud
#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>						//library for interrupt functions
#include "can_at90can128_AB_ID.c"

void USART0_ini(void);
void TIMER0_ini(void);

unsigned char UART_MSG[12] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char UART_MSG_02[13] = {0x80, 0x00, 0x00, 0x01, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned int timer0_cnt = 0;
unsigned char data_income = 0;
unsigned long data_id = 0;
unsigned char rec_data_position = 0;
unsigned char send_data_position = 0;
unsigned char send_data = 0;

can_message msg_01;
can_message msg_02;
can_message msg_03;
can_message msg_04;
can_message msg_05;
can_message msg_06;
can_message msg_07;
can_message msg_08;
// BMS status
can_message msg_09;
// Stack remaining capacity
can_message msg_10;
// Stack remaining range
can_message msg_11;
can_message msg_12;


SIGNAL (SIG_OVERFLOW0)	
{
	TCNT0 = 0x05;
	timer0_cnt++;
	
	if (timer0_cnt % 100 == 0)
	{

	}
}

SIGNAL (SIG_UART0_TRANS)								
{
	if (send_data == 1)
	{
		if (send_data_position < 13)
		{
			UDR0 = UART_MSG_02[send_data_position];
			send_data_position++;
		}
		else if (send_data_position == 13)
		{
			UDR0 = 0x0d;
			send_data = 0;
		}
	}
}

SIGNAL (SIG_UART0_RECV)								
{
	unsigned char uart_buffer = 0;
	
	uart_buffer = UDR0;
	
	if (data_income == 0)
	{
		if (uart_buffer == 0x80)
		{
			data_income = 1;
			rec_data_position = 0;
		}
	}
	else
	{
		UART_MSG[rec_data_position] = uart_buffer;
		rec_data_position++;
		if (rec_data_position == 12)
		{
			data_income = 0;
			data_id = ((unsigned long)UART_MSG[0] << 24) + ((unsigned long)UART_MSG[1] << 16) + ((unsigned long)UART_MSG[2] << 8) + UART_MSG[3];
			if (data_id == 0x111)
			{
				msg_01.data[0] = UART_MSG[4];
				msg_01.data[1] = UART_MSG[5];
				msg_01.data[2] = UART_MSG[6];
				msg_01.data[3] = UART_MSG[7];
				msg_01.data[4] = UART_MSG[8];
				msg_01.data[5] = UART_MSG[9];
				msg_01.data[6] = UART_MSG[10];
				msg_01.data[7] = UART_MSG[11];
				can_send_data(0, msg_01);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x112)
			{
				msg_02.data[0] = UART_MSG[4];
				msg_02.data[1] = UART_MSG[5];
				msg_02.data[2] = UART_MSG[6];
				msg_02.data[3] = UART_MSG[7];
				msg_02.data[4] = UART_MSG[8];
				msg_02.data[5] = UART_MSG[9];
				msg_02.data[6] = UART_MSG[10];
				msg_02.data[7] = UART_MSG[11];
				can_send_data(1, msg_02);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x130)
			{
				msg_03.data[0] = UART_MSG[4];
				msg_03.data[1] = UART_MSG[5];
				msg_03.data[2] = UART_MSG[6];
				msg_03.data[3] = UART_MSG[7];
				msg_03.data[4] = UART_MSG[8];
				msg_03.data[5] = UART_MSG[9];
				msg_03.data[6] = UART_MSG[10];
				msg_03.data[7] = UART_MSG[11];
				can_send_data(2, msg_03);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x131)
			{
				msg_04.data[0] = UART_MSG[4];
				msg_04.data[1] = UART_MSG[5];
				msg_04.data[2] = UART_MSG[6];
				msg_04.data[3] = UART_MSG[7];
				msg_04.data[4] = UART_MSG[8];
				msg_04.data[5] = UART_MSG[9];
				msg_04.data[6] = UART_MSG[10];
				msg_04.data[7] = UART_MSG[11];
				can_send_data(3, msg_04);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x140)
			{
				msg_05.data[0] = UART_MSG[4];
				msg_05.data[1] = UART_MSG[5];
				msg_05.data[2] = UART_MSG[6];
				msg_05.data[3] = UART_MSG[7];
				msg_05.data[4] = UART_MSG[8];
				msg_05.data[5] = UART_MSG[9];
				msg_05.data[6] = UART_MSG[10];
				msg_05.data[7] = UART_MSG[11];
				can_send_data(4, msg_05);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x141)
			{
				msg_06.data[0] = UART_MSG[4];
				msg_06.data[1] = UART_MSG[5];
				msg_06.data[2] = UART_MSG[6];
				msg_06.data[3] = UART_MSG[7];
				msg_06.data[4] = UART_MSG[8];
				msg_06.data[5] = UART_MSG[9];
				msg_06.data[6] = UART_MSG[10];
				msg_06.data[7] = UART_MSG[11];
				can_send_data(5, msg_06);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x150)
			{
				msg_07.data[0] = UART_MSG[4];
				msg_07.data[1] = UART_MSG[5];
				msg_07.data[2] = UART_MSG[6];
				msg_07.data[3] = UART_MSG[7];
				msg_07.data[4] = UART_MSG[8];
				msg_07.data[5] = UART_MSG[9];
				msg_07.data[6] = UART_MSG[10];
				msg_07.data[7] = UART_MSG[11];
				can_send_data(6, msg_07);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x151)
			{
				msg_08.data[0] = UART_MSG[4];
				msg_08.data[1] = UART_MSG[5];
				msg_08.data[2] = UART_MSG[6];
				msg_08.data[3] = UART_MSG[7];
				msg_08.data[4] = UART_MSG[8];
				msg_08.data[5] = UART_MSG[9];
				msg_08.data[6] = UART_MSG[10];
				msg_08.data[7] = UART_MSG[11];
				can_send_data(7, msg_08);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x101)
			{
				msg_09.data[0] = UART_MSG[4];
				msg_09.data[1] = UART_MSG[5];
				msg_09.data[2] = UART_MSG[6];
				msg_09.data[3] = UART_MSG[7];
				msg_09.data[4] = UART_MSG[8];
				msg_09.data[5] = UART_MSG[9];
				msg_09.data[6] = UART_MSG[10];
				msg_09.data[7] = UART_MSG[11];
				can_send_data(8, msg_09);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x160)
			{
				msg_10.data[0] = UART_MSG[4];
				msg_10.data[1] = UART_MSG[5];
				msg_10.data[2] = UART_MSG[6];
				msg_10.data[3] = UART_MSG[7];
				msg_10.data[4] = UART_MSG[8];
				msg_10.data[5] = UART_MSG[9];
				msg_10.data[6] = UART_MSG[10];
				msg_10.data[7] = UART_MSG[11];
				can_send_data(9, msg_10);
				//can_send_data(1, msg2);
			}
			if (data_id == 0x161)
			{
				msg_11.data[0] = UART_MSG[4];
				msg_11.data[1] = UART_MSG[5];
				msg_11.data[2] = UART_MSG[6];
				msg_11.data[3] = UART_MSG[7];
				msg_11.data[4] = UART_MSG[8];
				msg_11.data[5] = UART_MSG[9];
				msg_11.data[6] = UART_MSG[10];
				msg_11.data[7] = UART_MSG[11];
				can_send_data(10, msg_11);
				//can_send_data(1, msg2);
			}
		}
	}
}

SIGNAL (SIG_CAN_INTERRUPT1)
{
	uint8_t	save_canpage;
 	static can_message message;
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
	message	= can_get_mob_data();
	// Id der Nachricht holen
	message.id = can_get_mob_id(can_get_mob_id_type());
	
	// Checking for message IDs
	if (message.id == 0x170)
	{
		UART_MSG_02[5] = message.data[0];
		UART_MSG_02[6] = message.data[1];
		UART_MSG_02[7] = message.data[2];
		UART_MSG_02[8] = message.data[3];
		UART_MSG_02[9] = message.data[4];
		UART_MSG_02[10] = message.data[5];
		UART_MSG_02[11] = message.data[6];
		UART_MSG_02[12] = message.data[7];
		
		send_data_position = 1;
		send_data = 1;
		UDR0 = UART_MSG_02[0];
	}
	// RXOK-Flag löschen
	clearbit(CANSTMOB, RXOK);
	// MOB auf Empfang und CAN 2.0B Standard setzen
	can_set_mob_mode(RECEIVE_DATA, can_get_mob_id_type());
	//CANPAGE wiederherstellen
	CANPAGE	= save_canpage;
}

int main(void)								//main program
{
	// CAN init, 125kbit/s and no interrupt enabled
	can_init(125, RX);
	// CAN test message
	msg_01.id = 0x111;
	msg_01.idm = 0xfff;
	msg_01.id_type = 0;
	msg_01.length = 8;
	
	msg_02.id = 0x112;
	msg_02.idm = 0xfff;
	msg_02.id_type = 0;
	msg_02.length = 8;
	
	msg_03.id = 0x130;
	msg_03.idm = 0xfff;
	msg_03.id_type = 0;
	msg_03.length = 8;
	
	msg_04.id = 0x131;
	msg_04.idm = 0xfff;
	msg_04.id_type = 0;
	msg_04.length = 8;
	
	msg_05.id = 0x140;
	msg_05.idm = 0xfff;
	msg_05.id_type = 0;
	msg_05.length = 8;
	
	msg_06.id = 0x141;
	msg_06.idm = 0xfff;
	msg_06.id_type = 0;
	msg_06.length = 8;
	
	msg_07.id = 0x150;
	msg_07.idm = 0xfff;
	msg_07.id_type = 0;
	msg_07.length = 8;
	
	msg_08.id = 0x151;
	msg_08.idm = 0xfff;
	msg_08.id_type = 0;
	msg_08.length = 8;

	msg_09.id = 0x101;
	msg_09.idm = 0xfff;
	msg_09.id_type = 0;
	msg_09.length = 8;	
	
	msg_10.id = 0x160;
	msg_10.idm = 0xfff;
	msg_10.id_type = 0;
	msg_10.length = 8;	
	
	msg_11.id = 0x161;
	msg_11.idm = 0xfff;
	msg_11.id_type = 0;
	msg_11.length = 8;	

	msg_12.id = 0x170;
	msg_12.idm = 0xfff;
	msg_12.id_type = 0;
	msg_12.length = 8;	

	can_enable_mob(0, TRANSMIT_DATA, msg_01);
	can_enable_mob(1, TRANSMIT_DATA, msg_02);
	can_enable_mob(2, TRANSMIT_DATA, msg_03);
	can_enable_mob(3, TRANSMIT_DATA, msg_04);
	can_enable_mob(4, TRANSMIT_DATA, msg_05);
	can_enable_mob(5, TRANSMIT_DATA, msg_06);
	can_enable_mob(6, TRANSMIT_DATA, msg_07);
	can_enable_mob(7, TRANSMIT_DATA, msg_08);
	can_enable_mob(8, TRANSMIT_DATA, msg_09);
	can_enable_mob(9, TRANSMIT_DATA, msg_10);
	can_enable_mob(10, TRANSMIT_DATA, msg_11);
	can_enable_mob(11, RECEIVE_DATA, msg_12);

	sei();										//global interrupt enable on

	TIMER0_ini();
	USART0_ini();

	while(1)
	{
	}
}	

void USART0_ini (void)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (BAUD>>8);
	UBRR0L = (unsigned char) BAUD;
	UCSR0C = (0 << UMSEL1) | //asynchronous
			(0 << UPM10) | (0<<UPM01) |	//Mode 00: no parity
			(0 << USBS1) | 	//1 stop bit
			(1 << UCSZ11) | (1 << UCSZ00);	//Mode 011: 8 bit data
	/* Enable transmitter, disable receiver */
	UCSR0B = (1 << RXCIE1) | (1 << TXCIE1) | (1 << RXEN1) | (1 << TXEN1) | (0 << UCSZ12);
	UCSR0A = (0 << U2X1);
}

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x05;
}
