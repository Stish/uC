#define STR_LEN 20

#include "lib/io_fct.c"                                // ATmega128 MCU
#include "lib/LC7981.c"
#include <avr/signal.h>
#include <avr/interrupt.h>
//#include "lib/insel.c"
#include "lib/draw_penguin.c"
//#include "lib/layout.c"
//#include <delay.h>                                  // Delay functions

unsigned char getting_artist = 0;
unsigned char artist_pos = 0;
unsigned char artist[STR_LEN] = "Artist";
unsigned char button = 0;

ISR(USART0_RX_vect)
{
	unsigned char udr_buffer;
	unsigned char i = 0;
	udr_buffer = UDR0;

	if (udr_buffer == 255)
	{
		getting_artist = 0;
		for (artist_pos; artist_pos <= (STR_LEN-2); artist_pos++)
		{
			artist[artist_pos] = ' ';
		}
	}
	
	if (getting_artist)
	{
		artist[artist_pos] = udr_buffer;
		artist_pos++;
	}
	
	if ((udr_buffer == 2) || (artist_pos == (STR_LEN-1)))
	{
		getting_artist = 1;
		artist_pos = 0;
/*		for (i = 0; i <= (STR_LEN-2); i++)
		{
			artist[i] = ' ';
		}*/
	}
	
	if (!(getting_artist))
	{	
		UDR0 = button;
	}
	
//	UDR0 = 68;
}

int main(void)
{
	init_in_pin('b', 0);
	init_serial();
//	USART_Init(21);
	UCSR0B |= (1 << RXCIE);	//enable interrupt
	sei();
	
	reset_pin('a', 1);
	set_pin('a', 2);
	
	lcd_graphics_init();
	lcd_graphics_clear();
//	g_draw_rectangle(6, 5, 100, 30);
//	g_draw_rectangle(8, 7, 100, 30);
//	g_draw_rectangle(10, 9, 100, 30);
//	g_draw_rectangle(12, 11, 100, 30);
//	g_draw_rectangle(140, 60, 20, 20);
//	g_draw_string(17, 15, "Graphics Demo!\nHello World!"); 
	draw_penguin();
//	g_draw_string(22, 44, "!\"#$%&'=\n()*+,-./\n:;<>?@\[\n]^_`|{}");
//	draw_pic();
//	g_draw_rectangle(1, 1, 160, 80);
	while(1)													// Loop Continue
	{
		if (!(getting_artist))
		{
			g_draw_string(17, 15, artist);
		}
		button = get_in_pin('b', 0);
	} 
}	
