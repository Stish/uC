// RX Module

// Display Data Port
#define LCD_DATA_DDR	DDRC
#define LCD_DATA_PORT	PORTC
#define LCD_DATA_PIN	PINC
// Display Control Port
#define LCD_CTRL_DDR	DDRA
#define LCD_CTRL_PORT	PORTA
// Display Control Pins
#define LCD_CTRL_RS	7
#define LCD_CTRL_RW	5
#define LCD_CTRL_E	6
// Display Size 
#define LCD_WIDTH	160
#define LCD_HEIGHT	80

#define AD_PIN_1 0								//connect to ad0
#define AD_PIN_2 1								//connect to ad1
#define BAUD 103	//UBRR0 for 16MHz, U2X0 = 0
#define F_CPU	16000000

#include <avr/io.h>
#include "lib/LC7981.c"
#include <avr/interrupt.h>						//library for interrupt functions
#include "lib/touch_pictures.c"					//library for cover picture function
#include "can_at90can128.c"

void USART1_ini (void);
void TIMER0_ini (void);

unsigned char val_ad_01_bcd[6] = "5.00V";
unsigned char val_ad_02_bcd[6] = "5.00A";
unsigned char timer0_cnt = 0;
unsigned char send_data = 0;
unsigned char send_data_position = 0;

can_message msg_rec;

SIGNAL (SIG_OVERFLOW0)								//a/d converter in interrupt mode
{
	TCNT0 = 0x05;
	timer0_cnt++;
	if (timer0_cnt == 250)
	{
		timer0_cnt = 0;
		send_data = 1;
		UDR1 = val_ad_01_bcd[send_data_position];
		send_data_position++;
	}
}

SIGNAL (SIG_UART1_TRANS)								//a/d converter in interrupt mode
{
	if (send_data == 1)
	{
		if (send_data_position < 5)
		{
			UDR1 = val_ad_01_bcd[send_data_position];
			send_data_position++;
		}
		else if (send_data_position == 5)
		{
			UDR1 = ';';
			send_data_position++;
		}
		else if (send_data_position < 11)
		{
			UDR1 = val_ad_02_bcd[send_data_position - 6];
			send_data_position++;
		}
		else
		{
			UDR1 = 0x0d;
			send_data_position = 0;
			send_data = 0;
		}
	}
}

int main(void)								//main program
{
	unsigned long val_power = 0;
	unsigned char i = 0;
	unsigned char val_ad_01_procent, val_ad_01_procent_buffer;
	
	unsigned char val_ad_01_procent_bcd[5] = "100%";
	unsigned char val_power_bcd[8] = "10.000W";
	
	DDRE |= (1 << 6);
	PORTE |= (0x01 << 6);
	
	can_init(125, RX);
	
	msg_rec.id = 0x02345678;
	msg_rec.idm = 0xffffffff;
	// Objekt 0 auf Empfang setzen
	//CAN_enableMOB (0, RECEIVE_DATA, msg);
	can_enable_mob(0, RECEIVE_DATA, msg_rec);

	PORTE &= ~(0x01 << 6);
	PORTE |= (0x01 << 6);
//============= 		Init LCD		================	
	lcd_graphics_init();						//init LCD
	lcd_graphics_clear();						//clear LCD
//============= 		Cover Picture	================
	draw_frame();									//draw cover picture and wait for touch input

	sei();										//global interrupt enable on
	USART1_ini();
	TIMER0_ini();
	
	g_draw_string(110, 2, "Voltage:");
	g_draw_string(110, 35, "Current:");
	g_draw_string(110, 57, "Power:");

	while(1)
	{
		g_draw_string(110, 12, val_ad_01_bcd);
		g_draw_string(110, 45, val_ad_02_bcd);
		g_draw_string(110, 67, val_power_bcd);	
		
		val_ad_01_procent = (((val_ad_01_bcd[0] - 0x30) * 100) + ((val_ad_01_bcd[2] - 0x30) * 10) + ((val_ad_01_bcd[3] - 0x30) * 1)) / 5;
		val_ad_01_procent_buffer = val_ad_01_procent;
		val_ad_01_procent_bcd[0] = (val_ad_01_procent_buffer / 100) + 0x30;
		val_ad_01_procent_buffer = val_ad_01_procent_buffer % 100;
		val_ad_01_procent_bcd[1] = (val_ad_01_procent_buffer / 10) + 0x30;
		val_ad_01_procent_buffer = val_ad_01_procent_buffer % 10;
		val_ad_01_procent_bcd[2] = val_ad_01_procent_buffer + 0x30;
		g_draw_string(110, 22, val_ad_01_procent_bcd);

		val_power = (((val_ad_01_bcd[0] - 0x30) * 100) + ((val_ad_01_bcd[2] - 0x30) * 10) + ((val_ad_01_bcd[3] - 0x30) * 1));
		val_power *= (((val_ad_02_bcd[0] - 0x30) * 100) + ((val_ad_02_bcd[2] - 0x30) * 10) + ((val_ad_02_bcd[3] - 0x30) * 1));
		val_power /= 10;
		
		val_power_bcd[0] = (val_power / 10000) + 0x30;
		val_power = val_power % 10000;
		val_power_bcd[1] = (val_power / 1000) + 0x30;
		val_power = val_power % 1000;
		val_power_bcd[3] = (val_power / 100) + 0x30;
		val_power = val_power % 100;
		val_power_bcd[4] = (val_power / 10) + 0x30;
		val_power = val_power % 10;
		val_power_bcd[5] = (val_power / 1) + 0x30;
		g_draw_string(110, 67, val_power_bcd);	
		
		for (i = 1; i <= (val_ad_01_procent / 10); i++)
		{
			draw_bar(67, 72 - (5*i));
		}
		for (i = 1; i <= (10 - (val_ad_01_procent / 10)); i++)
		{
			delete_bar(67, 17 + (5*i));
		}
	}
}	

void USART1_ini (void)
{
	/* Set baud rate */
	UBRR1H = (unsigned char) (BAUD>>8);
	UBRR1L = (unsigned char) BAUD;
	UCSR1C = (0 << UMSEL1) | //asynchronous
			(0 << UPM10) | (0<<UPM01) |	//Mode 00: no parity
			(0 << USBS1) | 	//1 stop bit
			(1 << UCSZ11) | (1 << UCSZ00);	//Mode 011: 8 bit data
	/* Enable transmitter, disable receiver */
	UCSR1B = (0 << RXCIE1) | (1 << TXCIE1) | (0 << RXEN1) | (1 << TXEN1) | (0 << UCSZ12);
	UCSR1A = (0 << U2X1);
}

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x05;
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
	if(mob == NOMOB){
		return;
	}

	// Objekt das den Interrupt ausgelöst hat holen
	can_select_mob(mob);
    
	// Daten des MOBs aus CANMSG auslesen
	message			= can_get_mob_data();
	
	// Id der Nachricht holen
	message.id		= can_get_mob_id();


//	//////////////////////////////////
//	// Daten verarbeiten / ausgeben //
//	//////////////////////////////////
//
	if(message.id == 0x02345678){
		val_ad_01_bcd[0] = message.data[0];
		val_ad_01_bcd[2] = message.data[1];
		val_ad_01_bcd[3] = message.data[2];
		
		val_ad_02_bcd[0] = message.data[4];
		val_ad_02_bcd[2] = message.data[5];
		val_ad_02_bcd[3] = message.data[6];
	}

//	//////////////////////////////////
//	// Daten verarbeiten / ausgeben //
//	//////////////////////////////////

	// RXOK-Flag löschen
	clearbit(CANSTMOB, RXOK);
    
	// MOB auf Empfang und CAN 2.0B Standard setzen
	can_set_mob_mode(RECEIVE_DATA);
    
	//CANPAGE wiederherstellen
	CANPAGE		= save_canpage;
}