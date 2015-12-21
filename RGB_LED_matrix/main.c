//=== Device 0x60 ===
/*                       _____   _____
             _____      |     \_/     |
             Reset O--1-|PA2      VCC |-20-O
                        |             |
               RXD O--2-|PD0       PB7|-19-O UCSK/SCL/PCINT7
                        |             |
               TXD O--3-|PD1       PB6|-18-O MISO/DO/PCINT6
                        |             |
             XTAL2 O--4-|PA1       PB5|-17-O MOSI/DI/PCINT5
                        |             |
             XTAL1 O--5-|PA0       PB4|-16-O OC1B/PCINT4
                        |             |
    CKOUT/XCK/INT0 O--6-|PD2       PB3|-15-O OC1A/PCINT3
                        |             |
              INT1 O--7-|PD3       PB2|-14-O OC0A/PCINT2
                        |             |
                T0 O--8-|PD4       PB1|-13-O AIN1/PCINT1
                        |             |
           OC0B/T1 O--9-|PD5       PB0|-12-O AIN0/PCINT0
                        |             |
               GND O-10-|GND       PD6|-11-O ICP
                        |_____________|
                          ATTiny 2313
*/ 
/* Device numbers */
#define DEVICE_NUMBER_1 0x65	//a
#define DEVICE_NUMBER_2 0x66	//b
#define DEVICE_NUMBER_3 0x67	//c
#define DEVICE_NUMBER_4 0x68	//d
/* LED pins (Kathode) */
#define LED_1_RED		PD2
#define LED_1_GREEN		PA0
#define LED_1_BLUE		PA1

#define LED_2_RED		PD5
#define LED_2_GREEN		PD4
#define LED_2_BLUE		PD3

#define LED_3_RED		PB4
#define LED_3_GREEN		PB3
#define LED_3_BLUE		PB2

#define LED_4_RED		PB1
#define LED_4_GREEN		PB0
#define LED_4_BLUE		PD6

/* LED DDRs */
#define LED_1_RED_DDR	DDRD
#define LED_1_GREEN_DDR	DDRA
#define LED_1_BLUE_DDR	DDRA

#define LED_2_RED_DDR	DDRD
#define LED_2_GREEN_DDR	DDRD
#define LED_2_BLUE_DDR	DDRD

#define LED_3_RED_DDR	DDRB
#define LED_3_GREEN_DDR	DDRB
#define LED_3_BLUE_DDR	DDRB

#define LED_4_RED_DDR	DDRB
#define LED_4_GREEN_DDR	DDRB
#define LED_4_BLUE_DDR	DDRD
/* LED PORTs */
#define LED_1_RED_PORT		PORTD
#define LED_1_GREEN_PORT	PORTA
#define LED_1_BLUE_PORT		PORTA
#define LED_2_RED_PORT		PORTD
#define LED_2_GREEN_PORT	PORTD
#define LED_2_BLUE_PORT		PORTD
#define LED_3_RED_PORT		PORTB
#define LED_3_GREEN_PORT	PORTB
#define LED_3_BLUE_PORT		PORTB
#define LED_4_RED_PORT		PORTB
#define LED_4_GREEN_PORT	PORTB
#define LED_4_BLUE_PORT		PORTD
/* <Macros> */
#define LED_1_RED_INIT()		LED_1_RED_DDR|=(0x01<<LED_1_RED)
#define LED_1_GREEN_INIT()		LED_1_GREEN_DDR|=(0x01<<LED_1_GREEN)
#define LED_1_BLUE_INIT()		LED_1_BLUE_DDR|=(0x01<<LED_1_BLUE)
#define LED_2_RED_INIT()		LED_2_RED_DDR|=(0x01<<LED_2_RED)
#define LED_2_GREEN_INIT()		LED_2_GREEN_DDR|=(0x01<<LED_2_GREEN)
#define LED_2_BLUE_INIT()		LED_2_BLUE_DDR|=(0x01<<LED_2_BLUE)
#define LED_3_RED_INIT()		LED_3_RED_DDR|=(0x01<<LED_3_RED)
#define LED_3_GREEN_INIT()		LED_3_GREEN_DDR|=(0x01<<LED_3_GREEN)
#define LED_3_BLUE_INIT()		LED_3_BLUE_DDR|=(0x01<<LED_3_BLUE)
#define LED_4_RED_INIT()		LED_4_RED_DDR|=(0x01<<LED_4_RED)
#define LED_4_GREEN_INIT()		LED_4_GREEN_DDR|=(0x01<<LED_4_GREEN)
#define LED_4_BLUE_INIT()		LED_4_BLUE_DDR|=(0x01<<LED_4_BLUE)

#define LED_1_RED_OFF()			LED_1_RED_PORT|=(0x01<<LED_1_RED)
#define LED_1_GREEN_OFF()		LED_1_GREEN_PORT|=(0x01<<LED_1_GREEN)
#define LED_1_BLUE_OFF()		LED_1_BLUE_PORT|=(0x01<<LED_1_BLUE)
#define LED_2_RED_OFF()			LED_2_RED_PORT|=(0x01<<LED_2_RED)
#define LED_2_GREEN_OFF()		LED_2_GREEN_PORT|=(0x01<<LED_2_GREEN)
#define LED_2_BLUE_OFF()		LED_2_BLUE_PORT|=(0x01<<LED_2_BLUE)
#define LED_3_RED_OFF()			LED_3_RED_PORT|=(0x01<<LED_3_RED)
#define LED_3_GREEN_OFF()		LED_3_GREEN_PORT|=(0x01<<LED_3_GREEN)
#define LED_3_BLUE_OFF()		LED_3_BLUE_PORT|=(0x01<<LED_3_BLUE)
#define LED_4_RED_OFF()			LED_4_RED_PORT|=(0x01<<LED_4_RED)
#define LED_4_GREEN_OFF()		LED_4_GREEN_PORT|=(0x01<<LED_4_GREEN)
#define LED_4_BLUE_OFF()		LED_4_BLUE_PORT|=(0x01<<LED_4_BLUE)

#define LED_1_RED_ON()			LED_1_RED_PORT&=~(0x01<<LED_1_RED)
#define LED_1_GREEN_ON()		LED_1_GREEN_PORT&=~(0x01<<LED_1_GREEN)
#define LED_1_BLUE_ON()			LED_1_BLUE_PORT&=~(0x01<<LED_1_BLUE)
#define LED_2_RED_ON()			LED_2_RED_PORT&=~(0x01<<LED_2_RED)
#define LED_2_GREEN_ON()		LED_2_GREEN_PORT&=~(0x01<<LED_2_GREEN)
#define LED_2_BLUE_ON()			LED_2_BLUE_PORT&=~(0x01<<LED_2_BLUE)
#define LED_3_RED_ON()			LED_3_RED_PORT&=~(0x01<<LED_3_RED)
#define LED_3_GREEN_ON()		LED_3_GREEN_PORT&=~(0x01<<LED_3_GREEN)
#define LED_3_BLUE_ON()			LED_3_BLUE_PORT&=~(0x01<<LED_3_BLUE)
#define LED_4_RED_ON()			LED_4_RED_PORT&=~(0x01<<LED_4_RED)
#define LED_4_GREEN_ON()		LED_4_GREEN_PORT&=~(0x01<<LED_4_GREEN)
#define LED_4_BLUE_ON()			LED_4_BLUE_PORT&=~(0x01<<LED_4_BLUE)
/* </Macros> */

/* Includes */
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
void vUART_init(void);
/* Global variables */
unsigned int ovrflw_cnt = 1;
unsigned char rgb_buffer[3] = 	{0x00, 0x00, 0x00};
unsigned char rgb[4][3] = 	{	{0x00, 0x00, 0x00}, 
								{0x00, 0x00, 0x00}, 
								{0x00, 0x00, 0x00}, 
								{0x00, 0x00, 0x00}};
unsigned char rec_char = 0;
unsigned char rec_char_buffer = 0;
unsigned char data_incoming = 0;
unsigned char i = 0;
unsigned char led_select = 0;
/* Interrupts */
SIGNAL (USART_RX_vect)
{
	rec_char_buffer = UDR;
	if (data_incoming > 0)
	{
		rgb_buffer[i] = rec_char_buffer;
		i++;
		if (i == 3)
		{
			rgb[data_incoming - 1][0] = rgb_buffer[0];
			rgb[data_incoming - 1][1] = rgb_buffer[1];
			rgb[data_incoming - 1][2] = rgb_buffer[2];
			data_incoming = 0;
		}	
	}
	switch (rec_char_buffer)
	{
		case DEVICE_NUMBER_1:
			data_incoming = 1;
			i = 0;
			break;
		case DEVICE_NUMBER_2:
			data_incoming = 2;
			i = 0;
			break;
		case DEVICE_NUMBER_3:
			data_incoming = 3;
			i = 0;
			break;
		case DEVICE_NUMBER_4:
			data_incoming = 4;
			i = 0;
			break;
		default:
			break;
	}
} 
/* Main function */
int main(void)
{
	LED_1_RED_INIT();
	LED_1_GREEN_INIT();
	LED_1_BLUE_INIT();
	LED_2_RED_INIT();
	LED_2_GREEN_INIT();
	LED_2_BLUE_INIT();
	LED_3_RED_INIT();
	LED_3_GREEN_INIT();
	LED_3_BLUE_INIT();
	LED_4_RED_INIT();
	LED_4_GREEN_INIT();
	LED_4_BLUE_INIT();

	LED_1_RED_OFF();
	LED_1_GREEN_OFF();
	LED_1_BLUE_OFF();
	LED_2_RED_OFF();
	LED_2_GREEN_OFF();
	LED_2_BLUE_OFF();
	LED_3_RED_OFF();
	LED_3_GREEN_OFF();
	LED_3_BLUE_OFF();
	LED_4_RED_OFF();
	LED_4_GREEN_OFF();
	LED_4_BLUE_OFF();
	
	vUART_init();
	//Set global interrupt enable
	sei();
	//Endless loop
	while(1)
	{
		if (ovrflw_cnt == (300 + 1))
		{
			ovrflw_cnt = 1;
		}
	/* LED 1 */
	//=== Red LED ===
		if ((ovrflw_cnt >= 1) && (ovrflw_cnt <= rgb[0][0]))
		{
			LED_1_RED_ON();
		}
		else
		{
			LED_1_RED_OFF();
		}
	//=== Green LED ===
		if ((ovrflw_cnt >= 100) && (ovrflw_cnt < (100 + rgb[0][1])))
		{
			LED_1_GREEN_ON();
		}
		else
		{
			LED_1_GREEN_OFF();
		}
	//=== Blue LED ===
		if ((ovrflw_cnt >= 200) && (ovrflw_cnt < (200 + rgb[0][2])))
		{
			LED_1_BLUE_ON();
		}
		else
		{
			LED_1_BLUE_OFF();
		}
	/* LED 2 */	
	//=== Red LED ===
		if ((ovrflw_cnt >= 1) && (ovrflw_cnt <= rgb[1][0]))
		{
			LED_2_RED_ON();
		}
		else
		{
			LED_2_RED_OFF();
		}
	//=== Green LED ===
		if ((ovrflw_cnt >= 100) && (ovrflw_cnt < (100 + rgb[1][1])))
		{
			LED_2_GREEN_ON();
		}
		else
		{
			LED_2_GREEN_OFF();
		}
	//=== Blue LED ===
		if ((ovrflw_cnt >= 200) && (ovrflw_cnt < (200 + rgb[1][2])))
		{
			LED_2_BLUE_ON();
		}
		else
		{
			LED_2_BLUE_OFF();
		}
	/* LED 3 */	
	//=== Red LED ===
		if ((ovrflw_cnt >= 1) && (ovrflw_cnt <= rgb[2][0]))
		{
			LED_3_RED_ON();
		}
		else
		{
			LED_3_RED_OFF();
		}
	//=== Green LED ===
		if ((ovrflw_cnt >= 100) && (ovrflw_cnt < (100 + rgb[2][1])))
		{
			LED_3_GREEN_ON();
		}
		else
		{
			LED_3_GREEN_OFF();
		}
	//=== Blue LED ===
		if ((ovrflw_cnt >= 200) && (ovrflw_cnt < (200 + rgb[2][2])))
		{
			LED_3_BLUE_ON();
		}
		else
		{
			LED_3_BLUE_OFF();
		}
	/* LED 4 */	
	//=== Red LED ===
		if ((ovrflw_cnt >= 1) && (ovrflw_cnt <= rgb[3][0]))
		{
			LED_4_RED_ON();
		}
		else
		{
			LED_4_RED_OFF();
		}
	//=== Green LED ===
		if ((ovrflw_cnt >= 100) && (ovrflw_cnt < (100 + rgb[3][1])))
		{
			LED_4_GREEN_ON();
		}
		else
		{
			LED_4_GREEN_OFF();
		}
	//=== Blue LED ===
		if ((ovrflw_cnt >= 200) && (ovrflw_cnt < (200 + rgb[3][2])))
		{
			LED_4_BLUE_ON();
		}
		else
		{
			LED_4_BLUE_OFF();
		}
		ovrflw_cnt++;
	}
}

void vUART_init(void)
{
	//Ini USART
	//UBRRH = 0;		//UBRR to 51 for 9600 baud @ 8MHz
	//UBRRL = 51;
	UBRRH = 0;		//UBRR to 12 for 38400 baud @ 8MHz
	UBRRL = 12;
	UCSRA |= (1 << UDRE);	//Buffer is empty
	UCSRB |= (1 << RXCIE); 	//Interrupt enable for RX
	UCSRB |= (1 << RXEN);	//RX enable, pin is used as RX signal
	UCSRC |= (1 << UCSZ1);	//UCSZ2 = 0, UCSZ1 = 1, UCSZ0 = 1 : 8 data bits
	UCSRC |= (1 << UCSZ0);
	//UMSEL = 0 : asynchronous mode
	//UPM1 = 0, UPM0 = 0 : no parity
	//USBS = 0 : 1 stop bit
}